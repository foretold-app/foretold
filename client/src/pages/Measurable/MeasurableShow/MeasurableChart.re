open Utils;
open MomentRe;
let component = ReasonReact.statelessComponent("MeasurableChart");

let toUnix = x => x##createdAt |> Moment.toUnix;

module Styles = {
  open Css;
  let plot = style([maxWidth(px(500))]);
};

let onlyWithFloatCdf =
  filterAndFold((e, fnYes, fnNo) =>
    switch (e##value) {
    | Belt.Result.Ok(`FloatCdf(_)) => fnYes(e)
    | _ => fnNo()
    }
  );

let firstA = (_, n) => Some(n);

let make = (~measurements: MeasurableTypes.measurements, _children) => {
  ...component,
  render: _ => {
    let sorted =
      measurements
      |> E.A.O.concatSomes
      |> Js.Array.filter(e => e##value |> Belt.Result.isOk)
      |> Js_array.sortInPlaceWith((a, b) =>
           toUnix(b) > toUnix(a) ? (-1) : 1
         );

    let toChartMeasurement =
        (m: MeasurableTypes.measurement): option(TimeCdfChart.measurement) =>
      switch (m##value) {
      | Belt.Result.Ok(`FloatCdf(r)) =>
        switch (
          E.FloatCdf.firstAboveValue(0.05, r),
          E.FloatCdf.firstAboveValue(0.50, r),
          E.FloatCdf.firstAboveValue(0.95, r),
        ) {
        | (Some(low), Some(median), Some(high)) =>
          Some({
            createdAt: m##createdAt,
            competitorType: m##competitorType,
            low,
            median,
            high,
          })
        | _ => None
        }
      | _ => None
      };

    let values: array(TimeCdfChart.measurement) =
      sorted
      |> onlyWithFloatCdf
      |> E.A.fmap(toChartMeasurement)
      |> E.A.O.concatSomes;

    <div className=Styles.plot> <TimeCdfChart measurements=values /> </div>;
  },
};