open Utils;
open MomentRe;

let stringOfFloat = Js.Float.toPrecisionWithPrecision(_, ~digits=3);

let getFloatCdf = (e: Belt.Result.t(Value.t, string)) =>
  switch (e) {
  | Belt.Result.Ok(`FloatCdf(r)) => Some(r)
  | _ => None
  };

let bounds = (m: Js_array.t(DataModel.measurement)) => {
  let itemBounds =
    m
    |> Belt.Array.keepMap(_, r => getFloatCdf(r.value))
    |> Array.map(r =>
         (
           FloatCdf_F.firstAboveValue(0.05, r),
           FloatCdf_F.firstAboveValue(0.95, r),
         )
       );
  let min =
    itemBounds
    |> Belt.Array.keepMap(_, ((min, _)) => min)
    |> Array.fold_left((a, b) => a < b ? a : b, max_float);
  let max =
    itemBounds
    |> Belt.Array.keepMap(_, ((_, max)) => max)
    |> Array.fold_left((a, b) => a > b ? a : b, min_float);
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
    >>= FloatCdf_F.firstAboveValue(n)
    <$> stringOfFloat
    |> Rationale.Option.default("")
  );

let make = (ms: list(DataModel.measurement)) => {
  let bb = bounds(ms |> Array.of_list);
  ms
  |> List.sort((a: DataModel.measurement, b: DataModel.measurement) =>
       switch (a.createdAt, b.createdAt) {
       | (Some(c), Some(d)) =>
         Moment.toUnix(c) < Moment.toUnix(d) ? 1 : (-1)
       | (_, _) => 0
       }
     )
  |> List.map((m: DataModel.measurement) =>
       <div className={MeasurementTableStyles.row(~m)} key={m.id}>
         <div className=MeasurementTableStyles.mainColumn>
           <div className=MeasurementTableStyles.mainColumnTop>
             {MeasurementTableStyles.smallDistribution(m, bb)}
           </div>
         </div>
         <div className={MeasurementTableStyles.rightColumn(~m)}>
           <div className=MeasurementTableStyles.rightColumnInner>
             <div className=Style.Grid.Styles.flexColumn>
               <div className={Style.Grid.Styles.flex(1)}>
                 <div className=Style.Grid.Styles.flexRow>
                   <div className={Style.Grid.Styles.flex(4)}>
                     {MeasurementTableStyles.agentLink(~m)}
                   </div>
                   <div className={Style.Grid.Styles.flex(1)}>
                     {MeasurementTableStyles.relevantAt(~m)}
                   </div>
                 </div>
               </div>
               <div className={Style.Grid.Styles.flex(1)}>
                 {MeasurementTableStyles.description(~m)}
               </div>
             </div>
           </div>
         </div>
       </div>
     )
  |> Array.of_list
  |> ReasonReact.array;
};