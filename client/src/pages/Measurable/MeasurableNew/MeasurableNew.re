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

module SignUpForm = ReForm.Create(MeasurableForm.SignUpParams);

let mutate =
    (
      mutation: CreateMeasurableMutation.Mutation.apolloMutation,
      values: SignUpForm.values,
      channel: string,
    ) => {
  let mutate =
    values.showDescriptionDate == "TRUE" ?
      CreateMeasurableMutation.GraphQL.make(
        ~name=values.name,
        ~description=values.description,
        ~descriptionProperty=values.descriptionProperty,
        ~expectedResolutionDate=
          values.expectedResolutionDate |> Js.Json.string,
        ~resolutionEndpoint=values.resolutionEndpoint,
        ~descriptionEntity=values.descriptionEntity,
        ~descriptionDate=values.descriptionDate |> Js.Json.string,
        ~channel,
        ~valueType=`FLOAT,
        (),
      ) :
      CreateMeasurableMutation.GraphQL.make(
        ~name=values.name,
        ~description=values.description,
        ~descriptionProperty=values.descriptionProperty,
        ~expectedResolutionDate=
          values.expectedResolutionDate |> Js.Json.string,
        ~resolutionEndpoint=values.resolutionEndpoint,
        ~descriptionEntity=values.descriptionEntity,
        ~channel,
        ~valueType=`FLOAT,
        (),
      );
  mutation(
    ~variables=mutate##variables,
    ~refetchQueries=[|"getMeasurables"|],
    (),
  )
  |> ignore;
};

let component = ReasonReact.statelessComponent("Measurables");

let make = (~channel, _children) => {
  ...component,
  render: _ =>
    <div>
      <SLayout.Header>
        {SLayout.Header.textDiv("New Measurable")}
      </SLayout.Header>
      <SLayout.MainSection>
        {
          CreateMeasurableMutation.Mutation.make(
            ~onCompleted=e => Js.log("HI"),
            (mutation, data) =>
              SignUpForm.make(
                ~onSubmit=({values}) => mutate(mutation, values, channel),
                ~initialState={
                  name: "",
                  description: "",
                  descriptionProperty: "",
                  descriptionEntity: "",
                  expectedResolutionDate:
                    MomentRe.momentNow() |> MeasurableForm.formatDate,
                  descriptionDate:
                    MomentRe.momentNow() |> MeasurableForm.formatDate,
                  resolutionEndpoint: "",
                  showDescriptionDate: "FALSE",
                  showDescriptionProperty: "FALSE",
                },
                ~schema=[(`name, Custom(_ => None))],
                ({handleSubmit, handleChange, form, _}) =>
                  switch (data.result) {
                  | Loading => <div> {"Loading" |> ste} </div>
                  | Error(e) =>
                    <div>
                      {"Error: " ++ e##message |> ste}
                      {
                        MeasurableForm.showForm(
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
                    MeasurableForm.showForm(
                      ~form,
                      ~handleSubmit,
                      ~handleChange,
                    )
                  },
              )
              |> ReasonReact.element,
          )
          |> ReasonReact.element
        }
      </SLayout.MainSection>
    </div>,
};