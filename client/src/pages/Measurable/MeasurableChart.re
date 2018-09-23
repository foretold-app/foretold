open Utils;
open Rationale;
open Rationale.Option.Infix;
open Rationale.Function.Infix;
open Result.Infix;
open Queries;
open HandsOnTable;
open MomentRe;
open MeasurableTypes;
let component = ReasonReact.statelessComponent("MeasurableChart");

let toUnix = x => x##createdAt |> Moment.toUnix;

module Styles = {
  open Css;
  let plot = style([maxWidth(px(800))]);
};
/* 
let make = (~measurements: MeasurableTypes.measurements, _children) => {
  ...component,
  render: _ => {
    let sorted =
      measurements
      |> catOptionals
      |> Js_array.sortInPlaceWith((a, b) =>
           toUnix(b) > toUnix(a) ? (-1) : 1
         );

    let toTrio = e => e##value |> (r => r.trio);
    let toVal = (f, e) =>
      switch (e) {
      | Some(e) => f(e)
      | None => 0.0
      };

    let toPercentile = (f, e) => e |> toTrio |> toVal(f);
    let yMax =
      sorted
      |> Array.map(e => e |> toPercentile(e => e.p75))
      |> Array.fold_left((a, b) => a > b ? a : b, min_float);

    let yMin =
      sorted
      |> Array.map(e => e |> toPercentile(e => e.p25))
      |> Array.fold_left((a, b) => a < b ? a : b, max_float);

    let formatDate = Moment.format("MMM DD, YYYY HH:MM:SS");

    let xMax =
      sorted
      |> Array.map(e => e##createdAt)
      |> Array.fold_left(
           (a, b) => Moment.isAfter(a, b) ? a : b,
           "Jan 3, 1970" |> moment,
         )
      |> formatDate
      |> Js.Date.fromString;

    let aggregatePercentiles =
      sorted
      |> Js.Array.filter(e => e##competitorType == `AGGREGATION)
      |> Array.map(e =>
           {
             "x": e##relevantAt |> formatDate |> Js.Date.fromString,
             "y": e |> toPercentile(n => n.p25),
             "y0": e |> toPercentile(n => n.p75),
           }
         );
    let competitives =
      sorted
      |> Js.Array.filter(e => e##competitorType == `COMPETITIVE)
      |> Array.map(e =>
           {
             "x": e##relevantAt |> formatDate |> Js.Date.fromString,
             "y1": e |> toPercentile(n => n.p25),
             "y2": e |> toPercentile(n => n.p50),
             "y3": e |> toPercentile(n => n.p75),
           }
         );
    let aggregateMedians =
      sorted
      |> Js.Array.filter(e => e##competitorType == `AGGREGATION)
      |> Array.map(e =>
           {
             "x": e##relevantAt |> formatDate |> Js.Date.fromString,
             "y": e |> toPercentile(n => n.p50),
           }
         );
    Victory.(
      <div className=Styles.plot>
        <VictoryChart
          scale={"x": "time"}
          maxDomain={"y": yMax, "x": xMax}
          minDomain={"y": yMin}>
          <VictoryArea
            data=aggregatePercentiles
            style={
              "data": {
                "fill": "f6f6f6",
              },
            }
          />
          <VictoryLine
            data=aggregateMedians
            style={
              "data": {
                "stroke": "#ddd",
                "strokeWidth": "1",
                "strokeDasharray": "4 4 4 4",
              },
            }
          />
          (
            competitives
            |> Array.mapi((i, e) =>
                 <VictoryMeasurement point=e key=(string_of_int(i)) />
               )
            |> ReasonReact.array
          )
        </VictoryChart>
      </div>
    );
  },
}; */