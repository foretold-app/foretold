open Utils;
open Rationale;
open Rationale.Option.Infix;
open Rationale.Function.Infix;
open Antd;
open Types;

let ste = ReasonReact.string;

let component = ReasonReact.statelessComponent("MeasurableShowForm");

let make = (~measurableId: string, ~isCreator: bool, _children) => {
  ...component,
  render: _ =>
    CreateMeasurementMutation.Mutation.make(
      ~onCompleted=e => Js.log("Request submitted"),
      (mutation, data) =>
        <CdfInput
          onSubmit={
            ((value, competitorType, description)) =>
              CreateMeasurementMutation.mutate(
                mutation,
                measurableId,
                value,
                competitorType,
                description,
              )
          }
          data
          isCreator
        />,
    )
    |> ReasonReact.element,
};