open Utils;
open Rationale;
open Rationale.Option.Infix;
open Rationale.Function.Infix;
open Antd;
open Types;

let ste = ReasonReact.string;

module CreateMeasurement = [%graphql
  {|
            mutation createMeasurement($value: SequelizeJSON!, $competitorType:competitorType!, $measurableId:String!) {
                createMeasurement(value: $value, competitorType: $competitorType, measurableId:$measurableId) {
                  createdAt
                }
            }
    |}
];

module CreateMeasurementMutation =
  ReasonApollo.CreateMutation(CreateMeasurement);

let mutate =
    (
      mutation: CreateMeasurementMutation.apolloMutation,
      measurableId: string,
      value: Value.t,
      competitorType: CdfInput.competitorType,
    ) => {
  let m =
    CreateMeasurement.make(
      ~measurableId,
      ~value=value |> Value.encode,
      ~competitorType,
      (),
    );
  mutation(~variables=m##variables, ~refetchQueries=[|"getMeasurable"|], ())
  |> ignore;
};

let component = ReasonReact.statelessComponent("MeasurableShowForm");

let make = (~measurableId: string, _children) => {
  ...component,
  render: _ =>
    CreateMeasurementMutation.make(
      ~onCompleted=e => Js.log("Request submitted"),
      (mutation, _) =>
        <CdfInput
          onSubmit=(
            ((value, competitorType)) =>
              mutate(mutation, measurableId, value, competitorType)
          )
        />,
    )
    |> ReasonReact.element,
};