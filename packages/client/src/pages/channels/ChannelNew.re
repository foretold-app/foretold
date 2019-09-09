open Rationale.Function.Infix;

module ChannelFormShower = ReForm.Create(ChannelForm.Params);

module CMutationForm =
  MutationForm.Make({
    type queryType = ChannelCreate.Query.t;
  });

let component = ReasonReact.statelessComponent("ChannelNew");

let make = (~layout=SLayout.FullPage.makeWithEl, _children) => {
  ...component,
  render: _ => {
    let mutationMake =
      ChannelCreate.Mutation.make(~onCompleted=e => Js.log("HI"))
      ||> E.React.el;

    let form = mutation =>
      ChannelFormShower.make(
        ~onSubmit=
          ({values}) =>
            ChannelCreate.mutate(
              mutation,
              values.name,
              Some(values.description),
              values.isPublic == "TRUE" ? true : false,
              values.isArchived == "TRUE" ? true : false,
            ),
        ~initialState={
          name: "",
          description: "",
          isPublic: "TRUE",
          isArchived: "FALSE",
        },
        ~schema=[(`name, Custom(_ => None))],
      )
      ||> E.React.el;

    let body =
      <FC.PageCard.BodyPadding>
        {mutationMake((mutation, data) =>
           form(mutation, ({handleSubmit, handleChange, form, _}) =>
             CMutationForm.showWithLoading2(
               ~result=data.result,
               ~form=
                 ChannelForm.showForm(
                   ~form,
                   ~handleSubmit,
                   ~handleChange,
                   (),
                 ),
               ~onSuccess=
                 (response: ChannelCreate.Query.t) => {
                   switch (response##channelCreate) {
                   | Some(channel) =>
                     Routing.Url.push(ChannelShow(channel##id))
                   | _ => ()
                   };
                   ReasonReact.null;
                 },
               (),
             )
           )
         )}
      </FC.PageCard.BodyPadding>;

    SLayout.LayoutConfig.make(
      ~head=SLayout.Header.textDiv("Create a New Community"),
      ~body,
    )
    |> layout;
  },
};