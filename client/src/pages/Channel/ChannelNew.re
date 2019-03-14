open Utils;
open Rationale;
open Result.Infix;
open Rationale.Function.Infix;
open Antd;
open MomentRe;

let ste = ReasonReact.string;

module CreateMeasurableMutation = {
  module GraphQL = [%graphql
    {|
             mutation createMeasurable($name: String!, $description: String!, $valueType:valueType!, $expectedResolutionDate:Date, $resolutionEndpoint: String!, $descriptionEntity: String!, $descriptionDate: Date, $descriptionProperty: String, $channel: String) {
                 createMeasurable(name: $name, description: $description, valueType: $valueType, expectedResolutionDate: $expectedResolutionDate, resolutionEndpoint: $resolutionEndpoint, descriptionEntity: $descriptionEntity, descriptionDate: $descriptionDate, descriptionProperty: $descriptionProperty, channel: $channel) {
                   id
                 }
             }
     |}
  ];

  module Mutation = ReasonApollo.CreateMutation(GraphQL);
};

module ChannelFormShower = ReForm.Create(ChannelForm.SignUpParams);

let mutate =
    (
      mutation: CreateMeasurableMutation.Mutation.apolloMutation,
      values: ChannelFormShower.values,
      channel: string,
    ) => ignore;

let component = ReasonReact.statelessComponent("Measurables");

let make = (~channel="general", _children) => {
  ...component,
  render: _ =>
    CreateMeasurableMutation.Mutation.make(
      ~onCompleted=e => Js.log("HI"),
      (mutation, data) =>
        <div>
          <SLayout.Header>
            {SLayout.Header.textDiv("NewChannel")}
          </SLayout.Header>
          <SLayout.MainSection>
            {
              ChannelFormShower.make(
                ~onSubmit=({values}) => (),
                ~initialState={name: "", description: "", isPublic: "TRUE"},
                ~schema=[(`name, Custom(_ => None))],
                ({handleSubmit, handleChange, form, _}) =>
                  switch (data.result) {
                  | Loading => <div> {"Loading" |> ste} </div>
                  | Error(e) =>
                    <div>
                      {"Error: " ++ e##message |> ste}
                      {
                        ChannelForm.showForm(
                          ~form,
                          ~handleSubmit,
                          ~handleChange,
                        )
                      }
                    </div>
                  | Data(data) =>
                    data##createMeasurable
                    |> E.O.fmap(e => e##id)
                    |> doIfSome(_ => Urls.pushToLink(ChannelShow(channel)));
                    <h2> {"Measurable successfully created" |> ste} </h2>;
                  | NotCalled =>
                    ChannelForm.showForm(~form, ~handleSubmit, ~handleChange)
                  },
              )
              |> ReasonReact.element
            }
          </SLayout.MainSection>
        </div>,
    )
    |> ReasonReact.element,
};