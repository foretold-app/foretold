open Utils;
open Rationale;
open Result.Infix;
let component = ReasonReact.statelessComponent("Measurables");
let make = (~channel: string, ~loggedInUser: GetUser.t, _children) => {
  ...component,
  render: _self =>
    (
      measurables =>
        <MeasurableIndex__Table measurables loggedInUser showExtraData=true />
    )
    |> GetMeasurables.component(channel),
};