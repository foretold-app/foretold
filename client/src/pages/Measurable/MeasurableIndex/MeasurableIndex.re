open Utils;
open Rationale;
open Result.Infix;
let component = ReasonReact.statelessComponent("Measurables");
let make = (~channel: string, _children) => {
  ...component,
  render: _self =>
    (measurables => <MeasurableIndex__Table measurables />)
    |> Queries.GetMeasurables.component(channel),
};