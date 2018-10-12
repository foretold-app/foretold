open Utils;
open Rationale;
open Queries;
open HandsOnTable;
open MeasurableColumns;

let transformations = [
  name,
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
      Queries.GetMeasurablesQuery.make(o =>
        o.result
        |> apolloResponseToResult
        <$> (d => d##measurables)
        <$> Extensions.Array.concatSomes
        <$> Table.ColumnBundle.toHOT(~data=_, ~transformations)
        |> Result.result(idd, idd)
      )
      |> ReasonReact.element
      |> NormalLayout.make(~name="Measurables")
      |> ReasonReact.element
    ),
};