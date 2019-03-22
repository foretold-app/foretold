open Utils;
open Antd;
open MomentRe;

let ste = ReasonReact.string;

module CreateMeasurableMutation = {
  module GraphQL = [%graphql
    {|
             mutation createMeasurable($name: String!, $description: String!, $valueType:valueType!, $expectedResolutionDate:Date, $resolutionEndpoint: String!, $descriptionEntity: String!, $descriptionDate: Date, $descriptionProperty: String) {
                 createMeasurable(name: $name, description: $description, valueType: $valueType, expectedResolutionDate: $expectedResolutionDate, resolutionEndpoint: $resolutionEndpoint, descriptionEntity: $descriptionEntity, descriptionDate: $descriptionDate, descriptionProperty: $descriptionProperty) {
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
        <>
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
                    /* data##createMeasurable
                       |> E.O.fmap(e => e##id)
                       |> doIfSome(_ =>
                            DataModel.Url.push(ChannelShow(channel))
                          ); */
                    "Measurable successfully created" |> ste |> E.React.inH2
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