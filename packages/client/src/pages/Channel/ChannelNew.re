open Rationale.Function.Infix;

module ChannelFormShower = ReForm.Create(ChannelForm.NewChannelParams);

module Mutation = ChannelCreate;

module CMutationForm =
  MutationForm.Make({
    type queryType = Mutation.Query.t;
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
            Mutation.mutate(
              mutation,
              values.name,
              Some(values.description),
              values.isPublic == "TRUE" ? true : false,
            ),
        ~initialState={name: "", description: "", isPublic: "TRUE"},
        ~schema=[(`name, Custom(_ => None))],
      )
      ||> E.React.el;

    <FC.PageCard.BodyPadding>
      {mutationMake((mutation, data) =>
         form(mutation, ({handleSubmit, handleChange, form, _}) =>
           CMutationForm.showWithLoading(
             ~result=data.result,
             ~form=ChannelForm.showForm(~form, ~handleSubmit, ~handleChange),
             ~successMessage="Community created successfully.",
             (),
           )
         )
       )}
    </FC.PageCard.BodyPadding>
    |> SLayout.LayoutConfig.make(
         ~head=SLayout.Header.textDiv("Create a New Community"),
         ~body=_,
       )
    |> layout;
  },
};