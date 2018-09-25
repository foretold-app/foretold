open Utils;
open Rationale;
open Rationale.Option.Infix;
open Rationale.Option;
open HandsOnTable;
open MomentRe;
open AgentTypes;

let component = ReasonReact.statelessComponent("MeasurableTable");

let toUnix = x => x##createdAt |> Moment.toUnix;

let toMeasurableLink = (m: AgentTypes.measurable) => {
  let id = m.id;
  let name = m.name;
  {j|<a href="/measurables/$id">$name</a>|j};
};

let make = (~measurements: array(AgentTypes.measurement), _children) => {
  ...component,
  render: _ => {
    let data =
      measurements
      |> Array.map(e => {
           let value =
             Belt.Result.mapWithDefault(
               e.value,
               "",
               Shared.Value.stringOfValue,
             );
           Js.Dict.fromList([
             ("createdAt", e.createdAt |> Moment.format("L, h:mm:ss a")),
             ("value", value),
             (
               "measurable",
               e.measurable <$> toMeasurableLink |> Option.default(""),
             ),
           ]);
         });

    let columns = [|
      makeColumn(~data="createdAt", ()),
      makeColumn(~data="value", ()),
      makeColumn(~data="measurable", ~renderer="html", ()),
    |];
    let colHeaders = [|"Created at", "competitive", "Measurable"|];
    <UseRouterForLinks>
      <HandsOnTable data columns colHeaders />
    </UseRouterForLinks>;
  },
};