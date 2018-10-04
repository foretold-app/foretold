open Utils;
open Rationale;
open Rationale.Option.Infix;
open Rationale.Function.Infix;
open Antd;
open Types;

let ste = ReasonReact.string;

module CreateMeasurement = [%graphql
  {|
            mutation createMeasurement($value: SequelizeJSON!, $competitorType:competitorType!, $measurableId:String!, $agentId:String!) {
                createMeasurement(value: $value, competitorType: $competitorType, measurableId:$measurableId, agentId:$agentId) {
                  createdAt
                }
            }
    |}
];

module CreateMeasurementMutation =
  ReasonApollo.CreateMutation(CreateMeasurement);

type action =
  | UpdateFloatPdf(floatCdf);

let keepIfValid = n => Value.isValid(n) ? Some(n) : None;
let mutate =
    (
      mutation: CreateMeasurementMutation.apolloMutation,
      measurableId: string,
      values: floatCdf,
    ) => {
  let value =
    `FloatCdf(Value.FloatCdf.fromArrays(values |> (e => (e.ys, e.xs))));
  let m =
    CreateMeasurement.make(
      ~measurableId,
      ~agentId="c4aefed8-83c1-422d-9364-313071287758",
      ~value=value |> Value.encode,
      ~competitorType=`COMPETITIVE,
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
        <CdfInput onSubmit=(e => mutate(mutation, measurableId, e)) />,
    )
    |> ReasonReact.element,
};