open Utils;
open Rationale;
open Queries;
open HandsOnTable;
open MeasurableColumns;

let transformations = [
  link,
  nameAsText,
  measurementCount,
  measurerCount,
  expectedResolutionDate,
  creator,
  isLocked,
];

let component = ReasonReact.statelessComponent("Measurables");
let make = _children => {
  ...component,
  render: _self =>
    Result.Infix.(
      Queries.GetMeasurables.QueryComponent.make(o =>
        o.result
        |> apolloResponseToResult
        <$> (d => d##measurables)
        <$> Extensions.Array.concatSomes
        <$> (d => d |> Array.map(Queries.GetMeasurables.toMeasurable))
        <$> (measurables => <MeasurableIndex__Table measurables />)
        |> Result.result(idd, idd)
      )
      |> ReasonReact.element
      |> NormalLayout.make(~name="Measurables")
      |> ReasonReact.element
    ),
};