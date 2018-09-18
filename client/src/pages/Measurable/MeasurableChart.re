open Utils;
open Rationale;
open Rationale.Option.Infix;
open Rationale.Function.Infix;
open Result.Infix;
open Queries;
open HandsOnTable;
open MomentRe;
let component = ReasonReact.statelessComponent("MeasurableChart");

let toUnix = x => x##createdAt |> Moment.toUnix;

/* let make = (~measurements: MeasurableTypes.measurements, _children) => {
     ...component,
     render: _ => {
       let sorted =
         measurements
         |> catOptionals
         |> Js_array.sortInPlaceWith((a, b) =>
              toUnix(b) > toUnix(a) ? (-1) : 1
            );

       let yMax =
         sorted
         |> Array.map(e => e##percentile75 |> float_of_string)
         |> Array.fold_left((a, b) => a > b ? a : b, min_float);

       let yMin =
         sorted
         |> Array.map(e => e##percentile25 |> float_of_string)
         |> Array.fold_left((a, b) => a < b ? a : b, max_float);

       let percentiles =
         sorted
         |> Array.map(e =>
              {
                "x":
                  e##createdAt
                  |> Moment.format("MMM DD, YYYY HH:MM:ss:SSS")
                  |> Js.Date.fromString,
                "y": e##percentile25 |> float_of_string,
                "y0": e##percentile75 |> float_of_string,
              }
            );
       let all =
         sorted
         |> Array.map(e =>
              {
                "x":
                  e##createdAt
                  |> Moment.format("MMM DD, YYYY HH:MM:ss:SSS")
                  |> Js.Date.fromString,
                "y1": e##percentile25 |> float_of_string,
                "y2": e##percentile50 |> float_of_string,
                "y3": e##percentile75 |> float_of_string,
              }
            );
       let medians =
         sorted
         |> Array.map(e =>
              {
                "x":
                  e##createdAt
                  |> Moment.format("MMM DD, YYYY HH:MM:ss:SSS")
                  |> Js.Date.fromString,
                "y": e##percentile50 |> float_of_string,
              }
            );
       Victory.(
         <VictoryChart
           scale={"x": "time"} maxDomain={"y": yMax} minDomain={"y": yMin}>
           <VictoryArea data=percentiles style={
                                           "data": {
                                             "fill": "f6f6f6",
                                           },
                                         } />
           <VictoryLine
             data=medians
             style={
               "data": {
                 "stroke": "#ddd",
                 "strokeWidth": "1",
                 "strokeDasharray": "4 4 4 4",
               },
             }
           />
           (
             all
             |> Array.mapi((i, e) =>
                  <VictoryMeasurement point=e key=(string_of_int(i)) />
                )
             |> ReasonReact.array
           )
         </VictoryChart>
       );
     },
   }; */