open Utils;
open MomentRe;
open Style.Grid;

let stringOfFloat = Js.Float.toPrecisionWithPrecision(_, ~digits=3);

let getFloatCdf = (e: Belt.Result.t(Value.t, string)) =>
  switch (e) {
  | Belt.Result.Ok(`FloatCdf(r)) => Some(r)
  | _ => None
  };

let bounds = (m: Js_array.t(DataModel.Measurement.t)) => {
  let itemBounds =
    m
    |> E.A.keepMap(_, r => getFloatCdf(r.value))
    |> E.A.fmap(r =>
         (
           E.FloatCdf.firstAboveValue(0.05, r),
           E.FloatCdf.firstAboveValue(0.95, r),
         )
       );
  let min =
    itemBounds
    |> E.A.keepMap(_, ((min, _)) => min)
    |> E.A.fold_left((a, b) => a < b ? a : b, max_float);
  let max =
    itemBounds
    |> E.A.keepMap(_, ((_, max)) => max)
    |> E.A.fold_left((a, b) => a > b ? a : b, min_float);
  (min, max);
};

let floatCdf = e =>
  switch (e##value) {
  | Belt.Result.Ok(`FloatCdf(r)) => Some(r)
  | _ => None
  };

let percentile = (n, e) =>
  Rationale.Option.Infix.(
    floatCdf(e)
    >>= E.FloatCdf.firstAboveValue(n)
    <$> stringOfFloat
    |> E.O.default("")
  );

let toChartMeasurement = (m: DataModel.Measurement.t) =>
  switch (m.value) {
  | Belt.Result.Ok(`FloatCdf(r)) =>
    switch (
      E.FloatCdf.firstAboveValue(0.05, r),
      E.FloatCdf.firstAboveValue(0.95, r),
    ) {
    | (Some(low), Some(high)) => Some((low, high))
    | _ => None
    }
  | _ => None
  };

let toPercentiles = (m: DataModel.Measurement.t) =>
  switch (m.value) {
  | Belt.Result.Ok(`FloatCdf(r)) =>
    switch (
      E.FloatCdf.firstAboveValue(0.05, r),
      E.FloatCdf.firstAboveValue(0.95, r),
    ) {
    | (Some(low), Some(high)) =>
      let foo = stringOfFloat(low) ++ " to " ++ stringOfFloat(high);
      Some(foo);
    | _ => None
    }
  | _ => None
  };

module Styles = MeasurementTableStyles;

let stringOfFloat = Js.Float.toPrecisionWithPrecision(_, ~digits=3);

let make = (ms: list(DataModel.Measurement.t)) => {
  let _bounds = bounds(ms |> E.A.of_list);
  let items =
    ms
    |> E.L.sort((a: DataModel.Measurement.t, b: DataModel.Measurement.t) =>
         switch (a.createdAt, b.createdAt) {
         | (Some(c), Some(d)) =>
           Moment.toUnix(c) < Moment.toUnix(d) ? 1 : (-1)
         | _ => 0
         }
       )
    |> E.L.fmap((m: DataModel.Measurement.t) =>
         <div className={Styles.row(~m)} key={m.id}>
           <div className=Styles.mainColumn>
             <div className=Styles.mainColumnTop>
               {Styles.smallDistribution(m, _bounds)}
             </div>
             {
               switch (toPercentiles(m)) {
               | Some(a) =>
                 <div className={Styles.percentiles ++ " " ++ "foo"}>
                   {a |> ste}
                 </div>
               | _ => <span />
               }
             }
           </div>
           <div className={Styles.rightColumn(~m)}>
             <div className=Styles.rightColumnInner>
               <Div flexDirection=`column>
                 <Div flex=1>
                   <Div flexDirection=`row>
                     <Div flex=4> {Styles.agentLink(~m)} </Div>
                     <Div flex=1> {Styles.relevantAt(~m)} </Div>
                   </Div>
                 </Div>
                 <Div flex=1> {Styles.description(~m)} </Div>
               </Div>
             </div>
           </div>
         </div>
       )
    |> E.A.of_list
    |> ReasonReact.array;
  E.React.showIf(
    ms |> E.L.length > 0,
    <div className=Styles.group>
      items
      <div className=Styles.axisRow>
        <div className=Styles.mainColumn>
          <div className=Styles.mainColumnTop> <XAxis bounds=_bounds /> </div>
        </div>
        <div className=Styles.axisRightColumn />
      </div>
    </div>,
  );
};