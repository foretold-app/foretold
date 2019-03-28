open Utils;
open Antd;
open MomentRe;

let ste = ReasonReact.string;

module ChannelFormShower = ReForm.Create(ChannelForm.NewChannelParams);

let component = ReasonReact.statelessComponent("ChannelNewPage");

module Mutation = Foretold__GraphQL.Mutations.ChannelCreate;

let make = (~layout=SLayout.FullPage.makeWithEl, _children) => {
  ...component,
  render: _ =>
    SLayout.LayoutConfig.make(
      ~head=SLayout.Header.textDiv("NewChannel"),
      ~body=
        Mutation.Mutation.make(
          ~onCompleted=e => Js.log("HI"),
          (mutation, data) =>
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
              ({handleSubmit, handleChange, form, _}) =>
                switch (data.result) {
                | Loading => "Loading" |> ste
                | Error(e) =>
                  <>
                    {"Error: " ++ e##message |> ste}
                    {
                      ChannelForm.showForm(~form, ~handleSubmit, ~handleChange)
                    }
                  </>
                | Data(data) =>
                  "Channel successfully created" |> ste |> E.React.inH2
                | NotCalled =>
                  ChannelForm.showForm(~form, ~handleSubmit, ~handleChange)
                },
            )
            |> E.React.el,
        )
        |> E.React.el,
    )
    |> layout,
};