open Utils;
open Rationale;
open Rationale.Option.Infix;
open Rationale.Option;
open HandsOnTable;
open MomentRe;
open AgentTypes;

let component = ReasonReact.statelessComponent("MeasurableTable");

let toUnix = x => x##createdAt |> Moment.toUnix;

let make = (~measurements: array(AgentTypes.measurement), _children) => {
  ...component,
  render: _ => {
    let data =
      measurements
      |> Array.map(e =>
           Js.Dict.fromList([
             ("createdAt", e.createdAt |> Moment.format("L, h:mm:ss a")),
           ])
         );

    let columns = [|
      makeColumn(~data="createdAt", ()),
      makeColumn(~data="competitive", ()),
      makeColumn(~data="percentile25", ()),
      makeColumn(~data="percentile50", ()),
      makeColumn(~data="percentile75", ()),
    |];
    let colHeaders = [|"Created at", "competitive", "25th", "50th", "75th"|];
    <UseRouterForLinks>
      <HandsOnTable data columns colHeaders />
    </UseRouterForLinks>;
  },
};