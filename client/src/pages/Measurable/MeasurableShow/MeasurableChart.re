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

let onlyFloatPercentiles =
  filterAndFold((e, fnYes, fnNo) =>
    switch (e) {
    | Belt.Result.Ok(`FloatPercentiles(r)) => fnYes(r)
    | _ => fnNo()
    }
  );

let onlyWithFloatPercentiles =
  filterAndFold((e, fnYes, fnNo) =>
    switch (e##value) {
    | Belt.Result.Ok(`FloatPercentiles(r)) => fnYes(e)
    | _ => fnNo()
    }
  );

let make = (~measurements: MeasurableTypes.measurements, _children) => {
  ...component,
  render: _ => {
    let sorted =
      measurements
      |> Extensions.Array.concatSomes
      |> Js.Array.filter(e => e##value |> Belt.Result.isOk)
      |> Js_array.sortInPlaceWith((a, b) =>
           toUnix(b) > toUnix(a) ? (-1) : 1
         );

    let yMax =
      sorted
      |> Array.map(e => e##value)
      |> onlyFloatPercentiles
      |> Array.map(e => Belt.Map.get(e, 75.0))
      |> Extensions.Array.concatSomes
      |> Array.fold_left((a, b) => a > b ? a : b, min_float);

    let yMin =
      sorted
      |> Array.map(e => e##value)
      |> onlyFloatPercentiles
      |> Array.map(e => Belt.Map.get(e, 25.0))
      |> Extensions.Array.concatSomes
      |> Array.fold_left((a, b) => a < b ? a : b, max_float);

    let formatDate = Moment.format("MMM DD, YYYY HH:MM:SS");
    let toPercentage = (perc, m: measurement) =>
      switch (m##value) {
      | Belt.Result.Ok(e) =>
        switch (e) {
        | `FloatPoint(r) => r
        | `Percentage(r) => r
        | `FloatPercentiles(v) =>
          switch (Belt.Map.get(v, perc)) {
          | Some(e) => e
          | _ => 3.0
          }
        | _ => 5.0
        }
      | _ => 2.0
      };

    let xMax =
      sorted
      |> Array.map(e => e##createdAt)
      |> Array.fold_left(
           (a, b) => Moment.isAfter(a, b) ? a : b,
           "Jan 3, 1970" |> moment,
         )
      |> formatDate
      |> Js.Date.fromString;

    let xMin =
      sorted
      |> onlyWithFloatPercentiles
      |> Array.map(e => e##createdAt)
      |> Array.fold_left(
           (a, b) => Moment.isBefore(a, b) ? a : b,
           "Jan 3, 2070" |> moment,
         )
      |> formatDate
      |> Js.Date.fromString;

    let aggregatePercentiles =
      sorted
      |> onlyWithFloatPercentiles
      |> Js.Array.filter(e => e##competitorType == `AGGREGATION)
      |> Js.Array.filter(e =>
           switch (e##value) {
           | Belt.Result.Ok(`FloatPercentiles(_)) => true
           | _ => false
           }
         )
      |> Array.map(e =>
           {
             "y0": e |> toPercentage(25.0),
             "y": e |> toPercentage(75.0),
             "x": e##relevantAt |> formatDate |> Js.Date.fromString,
           }
         );
    let competitives =
      sorted
      |> onlyWithFloatPercentiles
      |> Js.Array.filter(e => e##competitorType == `COMPETITIVE)
      |> Array.map(e =>
           {
             "x": e##relevantAt |> formatDate |> Js.Date.fromString,
             "y1": e |> toPercentage(25.0),
             "y2": e |> toPercentage(50.0),
             "y3": e |> toPercentage(75.0),
           }
         );

    let aggregateMedians =
      sorted
      |> onlyWithFloatPercentiles
      |> Js.Array.filter(e => e##competitorType == `AGGREGATION)
      |> Array.map(e =>
           {
             "x": e##relevantAt |> formatDate |> Js.Date.fromString,
             "y": e |> toPercentage(50.0),
           }
         );

    Victory.(
      <div className=Styles.plot>
        <VictoryChart
          scale={"x": "time"}
          maxDomain={"y": yMax, "x": xMax}
          minDomain={"y": yMin, "x": xMin}>
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
};