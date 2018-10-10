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

let victory = data => <SmallChart data />;

let make = (~measurements: MeasurableTypes.measurements, _children) => {
  ...component,
  render: _ => {
    let data =
      measurements
      |> ArrayOptional.concatSomes
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
             Belt.Result.mapWithDefault(e##value, "", Value.stringOfValue);

           let presentableValueName = (t: Value.t) =>
             switch (t) {
             | `FloatPoint(_) => "Point"
             | `FloatCdf(_) => "Cdf"
             | `DateTimeCdf(_) => "Date Cdf"
             | `DateTimePoint(_) => "TimePoint"
             | `Percentage(_) => "Percentage"
             | `Binary(_) => "Binary"
             };

           let valueType =
             Belt.Result.mapWithDefault(e##value, "", presentableValueName);

           let floatCdf =
             switch (e##value) {
             | Belt.Result.Ok(`FloatCdf(r)) => Some(r)
             | _ => None
             };

           let cdfGraph =
             ReactDOMServerRe.renderToStaticMarkup(
               <div>
                 (
                   switch (floatCdf <$> Value.FloatCdf.toPoints) {
                   | Some(data) => <SmallChart data />
                   | None => "" |> ste
                   }
                 )
               </div>,
             );

           let stringOfFloat =
             Js.Float.toPrecisionWithPrecision(_, ~digits=3);

           let percentile = n =>
             floatCdf
             >>= FloatCdf_F.firstAboveValue(n)
             <$> stringOfFloat
             |> Option.default("");

           let middle =
             switch (e##value) {
             | Belt.Result.Ok(`FloatCdf(r)) => percentile(0.50)
             | Belt.Result.Ok(`FloatPoint(r)) => stringOfFloat(r)
             | _ => ""
             };

           Js.Dict.fromList([
             ("createdAt", e##relevantAt |> Moment.format("L, h:mm:ss a")),
             ("competitive", e##competitorType |> botCompetitor),
             ("value", value),
             ("valueType", valueType),
             ("botType", agentType |> botType),
             ("userLink", link(e##agent)),
             ("5th", percentile(0.05)),
             ("50th", middle),
             ("95th", percentile(0.95)),
             ("cdfGraph", cdfGraph),
           ]);
         });

    let columns = [|
      makeColumn(~data="createdAt", ()),
      makeColumn(~data="competitive", ()),
      makeColumn(~data="userLink", ~renderer="html", ()),
      makeColumn(~data="5th", ()),
      makeColumn(~data="50th", ()),
      makeColumn(~data="95th", ()),
      makeColumn(~data="cdfGraph", ~renderer="html", ()),
    |];
    let colHeaders = [|
      "Relevant at",
      "competitive",
      "Agent",
      "5th",
      "50th",
      "95th",
      "Cdf",
    |];
    <UseRouterForLinks>
      <HandsOnTable data columns colHeaders />
    </UseRouterForLinks>;
  },
};