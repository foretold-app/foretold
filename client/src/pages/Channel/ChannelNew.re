open Utils;
open Antd;
open MomentRe;
open Rationale.Function.Infix;

let ste = ReasonReact.string;

module ChannelFormShower = ReForm.Create(ChannelForm.NewChannelParams);

let component = ReasonReact.statelessComponent("ChannelNewPage");

module Mutation = Foretold__GraphQL.Mutations.ChannelCreate;

let toSetup = (f1, f2, fnlast) =>
  f1((mutation, data) => f2(mutation, form => fnlast(data, form)));

module CMutationForm =
  MutationForm.Make({
    type queryType = Mutation.Query.t;
  });

let make = (~layout=SLayout.FullPage.makeWithEl, _children) => {
  ...component,
  render: _ => {
    let mutationMake =
      Mutation.Mutation.make(~onCompleted=e => Js.log("HI")) ||> E.React.el;

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

    mutationMake((mutation, data) =>
      form(mutation, ({handleSubmit, handleChange, form, _}) =>
        CMutationForm.showWithLoading(
          ~result=data.result,
          ~form=ChannelForm.showForm(~form, ~handleSubmit, ~handleChange),
          ~successMessage="Channel created successfully.",
          (),
        )
      )
    )
    |> SLayout.LayoutConfig.make(
         ~head=SLayout.Header.textDiv("Create a New Channel"),
         ~body=_,
       )
    |> layout;
  },
};