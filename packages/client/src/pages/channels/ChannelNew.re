open Rationale.Function.Infix;

module CMutationForm =
  MutationForm.Make({
    type queryType = ChannelCreate.Query.t;
  });

let component = ReasonReact.statelessComponent("ChannelNew");

let make = _children => {
  ...component,
  render: _ => {
    let mutationMake =
      ChannelCreate.Mutation.make(~onCompleted=_e => Js.log("HI"))
      ||> E.React.el;

    let form = mutation =>
      ChannelForm.Form.make(
        ~onSubmit=
          values =>
            ChannelCreate.mutate(
              mutation,
              values.state.values.name,
              Some(values.state.values.description),
              values.state.values.isPublic == "TRUE" ? true : false,
              values.state.values.isArchived == "TRUE" ? true : false,
            ),
        ~initialState={
          name: "",
          description: "",
          isPublic: "TRUE",
          isArchived: "FALSE",
        },
        ~schema=ChannelForm.Form.Validation.Schema([||]),
      )
      ||> E.React.el;

    let body =
      <FC.PageCard.BodyPadding>
        {mutationMake((mutation, data) =>
           form(mutation, ({send, state}) =>
             CMutationForm.showWithLoading2(
               ~result=data.result,
               ~form=
                 ChannelForm.showForm(
                   ~state,
                   ~send,
                   ~onSubmit=() => send(ChannelForm.Form.Submit),
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

    <SLayout head={SLayout.Header.textDiv("Create a New Community")}>
      body
    </SLayout>;
  },
};