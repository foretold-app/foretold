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
];

let component = ReasonReact.statelessComponent("Measurables");
let make = (~channel: string, _children) => {
  ...component,
  render: _self => {
    open Result.Infix;
    let query =
      Queries.GetMeasurables.Query.make(~offset=0, ~limit=200, ~channel, ());
    Queries.GetMeasurables.QueryComponent.make(~variables=query##variables, o =>
      o.result
      |> ApolloUtils.apolloResponseToResult
      <$> (d => d##measurables)
      <$> Extensions.Array.concatSomes
      <$> (d => d |> Array.map(Queries.GetMeasurables.toMeasurable))
      <$> (measurables => <MeasurableIndex__Table measurables />)
      |> Result.result(idd, idd)
    )
    |> ReasonReact.element
    |> FillWithSidebar.make(~channel=Some(channel))
    |> ReasonReact.element;
  },
};