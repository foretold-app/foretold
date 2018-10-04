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

module Styles = {
  open Css;
  let plot = style([maxWidth(px(800))]);
};

type competitorType = [ | `AGGREGATION | `COMPETITIVE | `OBJECTIVE];

type measurement = {
  createdAt: MomentRe.Moment.t,
  competitorType,
  low: float,
  median: float,
  high: float,
};

let make = (~measurements: array(measurement), _children) => {
  ...component,
  render: _ => {
    let sorted = measurements;

    let yMax =
      sorted
      |> Array.map(e => e.high)
      |> Array.fold_left((a, b) => a > b ? a : b, min_float);

    let yMin =
      sorted
      |> Array.map(e => e.low)
      |> Array.fold_left((a, b) => a < b ? a : b, max_float);

    let formatDate = Moment.format("MMM DD, YYYY HH:MM:SS");
    let xMax =
      sorted
      |> Array.map(e => e.createdAt)
      |> Array.fold_left(
           (a, b) => Moment.isAfter(a, b) ? a : b,
           "Jan 3, 1970" |> moment,
         )
      |> formatDate
      |> Js.Date.fromString;

    let xMin =
      sorted
      |> Array.map(e => e.createdAt)
      |> Array.fold_left(
           (a, b) => Moment.isBefore(a, b) ? a : b,
           "Jan 3, 2070" |> moment,
         )
      |> formatDate
      |> Js.Date.fromString;

    let aggregatePercentiles =
      sorted
      |> Js.Array.filter(e => e.competitorType == `AGGREGATION)
      |> Array.map(e =>
           {
             "y0": e.low,
             "y": e.high,
             "x": e.createdAt |> formatDate |> Js.Date.fromString,
           }
         );
    let competitives =
      sorted
      |> Js.Array.filter(e => e.competitorType == `COMPETITIVE)
      |> Array.map(e =>
           {
             "x": e.createdAt |> formatDate |> Js.Date.fromString,
             "y1": e.low,
             "y2": e.median,
             "y3": e.high,
           }
         );

    let aggregateMedians =
      sorted
      |> Js.Array.filter(e => e.competitorType == `AGGREGATION)
      |> Array.map(e =>
           {
             "x": e.createdAt |> formatDate |> Js.Date.fromString,
             "y": e.median,
           }
         );

    Victory.(
      <div className=Styles.plot>
        <VictoryChart
          scale={"x": "time"}
          padding={"top": 10, "bottom": 25, "right": 10, "left": 30}
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