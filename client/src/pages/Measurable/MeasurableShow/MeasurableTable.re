open Utils;
open Rationale;
open Rationale.Option.Infix;
open Rationale.Option;
open Rationale;
open Queries;
open HandsOnTable;
open MomentRe;
open MeasurableTypes;

let component = ReasonReact.statelessComponent("MeasurableTable");

let toUnix = x => x##createdAt |> Moment.toUnix;

let make = (~measurements: MeasurableTypes.measurements, _children) => {
  ...component,
  render: _ => {
    let data =
      measurements
      |> catOptionals
      |> Js_array.sortInPlaceWith((a, b) =>
           toUnix(b) > toUnix(a) ? (-1) : 1
         )
      |> Array.map(e => {
           let link: 'a => string =
             agent =>
               switch (
                 agent <$> (x => x##id),
                 agent >>= (x => x##bot) >>= (x => x##name),
                 agent >>= (x => x##user) <$> (x => x##name),
               ) {
               | (None, _, _) => ""
               | (id, Some(name), _) => {j|<a href="/agents/$id">$name</a>|j}
               | (id, _, Some(name)) => {j|<a href="/agents/$id">$name</a>|j}
               | (_, _, _) => ""
               };

           let agentType: option(competitorType) =
             e##agent >>= (x => x##bot) <$> (x => x##competitorType);

           let botType = e =>
             switch (e) {
             | Some(`AGGREGATION) => "Aggregation"
             | Some(`COMPETITIVE) => "Competitive"
             | Some(`OBJECTIVE) => "Objective"
             | _ => ""
             };

           let botCompetitor = e =>
             switch (e) {
             | `AGGREGATION => "Aggregation"
             | `COMPETITIVE => "Competitive"
             | `OBJECTIVE => "Objective"
             };

           let value =
             Belt.Result.mapWithDefault(
               e##value,
               "",
               Shared.Value.stringOfValue,
             );

           let presentableValueName = (t: Shared.Value.t) =>
             switch (t) {
             | `FloatPoint(_) => "Point"
             | `FloatPercentiles(_) => "Percentiles"
             | `DateTimePercentiles(_) => "Date Percentiles"
             | `DateTimePoint(_) => "TimePoint"
             | `Percentage(_) => "Percentage"
             | `Binary(_) => "Binary"
             };

           let valueType =
             Belt.Result.mapWithDefault(e##value, "", presentableValueName);

           Js.Dict.fromList([
             ("createdAt", e##relevantAt |> Moment.format("L, h:mm:ss a")),
             ("competitive", e##competitorType |> botCompetitor),
             ("value", value),
             ("valueType", valueType),
             ("type", agentType |> botType),
             ("userLink", link(e##agent)),
           ]);
         });

    let columns = [|
      makeColumn(~data="createdAt", ()),
      makeColumn(~data="competitive", ()),
      makeColumn(~data="value", ()),
      makeColumn(~data="valueType", ()),
      makeColumn(~data="type", ()),
      makeColumn(~data="userLink", ~renderer="html", ()),
    |];
    let colHeaders = [|
      "Relevant at",
      "competitive",
      "Value",
      "Value Type",
      "Bot Type",
      "Agent",
    |];
    <UseRouterForLinks>
      <HandsOnTable data columns colHeaders />
    </UseRouterForLinks>;
  },
};