open Utils;
open Antd;
open MomentRe;

let ste = ReasonReact.string;

module ChannelFormShower = ReForm.Create(ChannelForm.NewChannelParams);

let component = ReasonReact.statelessComponent("ChannelNewPage");

module Mutation = Foretold__GraphQL.Mutations.ChannelCreate;

let make = _children => {
  ...component,
  render: _ =>
    Mutation.Mutation.make(
      ~onCompleted=e => Js.log("HI"),
      (mutation, data) =>
        <>
          <SLayout.Header>
            {SLayout.Header.textDiv("NewChannel")}
          </SLayout.Header>
          <SLayout.MainSection>
            {
              ChannelFormShower.make(
                ~onSubmit=
                  ({values}) =>
                    Mutation.mutate(
                      mutation,
                      values.name,
                      Some(values.description),
                      true,
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
                        ChannelForm.showForm(
                          ~form,
                          ~handleSubmit,
                          ~handleChange,
                        )
                      }
                    </>
                  | Data(data) =>
                    "Channel successfully created" |> ste |> E.React.inH2
                  | NotCalled =>
                    ChannelForm.showForm(~form, ~handleSubmit, ~handleChange)
                  },
              )
              |> E.React.el
            }
          </SLayout.MainSection>
        </>,
    )
    |> E.React.el,
};