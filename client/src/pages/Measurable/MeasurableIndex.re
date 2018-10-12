open Utils;
open Rationale;
open Queries;
open HandsOnTable;
open MeasurableColumns;

let transformations = [
  name,
  valueType,
  measurementCount,
  measurerCount,
  isLocked,
  expectedResolutionDate,
  creator,
];

let showQueryList = (~data) =>
  Table.ColumnBundle.toHOT(~data, ~transformations);

let component = ReasonReact.statelessComponent("Measurables");
let make = _children => {
  ...component,
  render: self =>
    Result.Infix.(
      Queries.GetMeasurablesQuery.make(o =>
        o.result
        |> apolloResponseToResult
        <$> (d => d##measurables)
        <$> Extensions.Array.concatSomes
        <$> (e => <div> (showQueryList(~data=e)) </div>)
        |> Result.result(idd, idd)
      )
      |> ReasonReact.element
      |> NormalLayout.make(~name="Measurables")
      |> ReasonReact.element
    ),
};