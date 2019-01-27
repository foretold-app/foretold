open Utils;
open Rationale;
open Rationale.Option.Infix;
open MomentRe;
open Table;

let component = ReasonReact.statelessComponent("Measurable__Table");

let toUnix = x => x##createdAt |> Moment.toUnix;

let victory = data => <SmallChart data />;

type tt = MeasurableTypes.measurement;
let foo = ColumnBundle.make(~headerName="Agent", ~get=(_: tt) => "sdf", ());

let botCompetitor = e =>
  switch (e) {
  | `AGGREGATION => "Aggregation"
  | `COMPETITIVE => "Competitive"
  | `OBJECTIVE => "Objective"
  };

let floatCdf = e =>
  switch (e##value) {
  | Belt.Result.Ok(`FloatCdf(r)) => Some(r)
  | _ => None
  };

let stringOfFloat = Js.Float.toPrecisionWithPrecision(_, ~digits=3);

let percentile = (n, e) =>
  floatCdf(e)
  >>= FloatCdf_F.firstAboveValue(n)
  <$> stringOfFloat
  |> Option.default("");

let transformations = [
  ColumnBundle.make(
    ~headerName="Agent",
    ~column=Columns.html,
    ~get=
      e => {
        let link = agent =>
          switch (agent <$> (x => x##id), agent <$> (x => x##name)) {
          | (id, Some(name)) => {j|<a href="/agents/$id">$name</a>|j}
          | (_, _) => ""
          };
        link(e##agent);
      },
    (),
  ),
  ColumnBundle.make(
    ~headerName="Description",
    ~get=e => e##description |> Rationale.Option.default(""),
    (),
  ),
  ColumnBundle.make(
    ~headerName="Competitive",
    ~get=e => e##competitorType |> botCompetitor,
    (),
  ),
  ColumnBundle.make(~headerName="5th", ~get=percentile(0.05), ()),
  ColumnBundle.make(
    ~headerName="50th",
    ~get=
      e =>
        switch (e##value) {
        | Belt.Result.Ok(`FloatCdf(_)) => percentile(0.50, e)
        | Belt.Result.Ok(`FloatPoint(r)) => stringOfFloat(r)
        | _ => ""
        },
    (),
  ),
  ColumnBundle.make(~headerName="95th", ~get=percentile(0.95), ()),
  ColumnBundle.make(
    ~headerName="Cdf",
    ~column=Columns.html,
    ~get=
      e =>
        ReactDOMServerRe.renderToStaticMarkup(
          <div>
            {
              switch (floatCdf(e) <$> Value.FloatCdf.toPoints) {
              | Some(data) => <SmallChart data />
              | None => "" |> ste
              }
            }
          </div>,
        ),
    (),
  ),
  ColumnBundle.make(
    ~headerName="Relevant at",
    ~get=
      (e: tt) =>
        e##relevantAt
        |> Rationale.Option.fmap(Moment.format("L, h:mm:ss a"))
        |> Rationale.Option.default(""),
    (),
  ),
];

let getFloatCdf = (e: Belt.Result.t(Value.t, string)) =>
  switch (e) {
  | Belt.Result.Ok(`FloatCdf(r)) => Some(r)
  | _ => None
  };

/* Js.Exn.raiseError("Invalid GraphQL State") */
let bounds = (m: Js_array.t(MeasurableTypes.measurement)) => {
  let itemBounds =
    m
    |> Belt.Array.keepMap(_, r => getFloatCdf(r##value))
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

let make = (~measurements: MeasurableTypes.measurements, _children) => {
  ...component,
  render: _ => {
    let data =
      measurements
      |> ArrayOptional.concatSomes
      |> Js_array.sortInPlaceWith((a, b) =>
           toUnix(b) > toUnix(a) ? (-1) : 1
         );

    let bb = bounds(data);
    <UseRouterForLinks>
      <div className=MeasurementTableStyles.group>
        {
          data
          |> Array.map(m =>
               <div className={MeasurementTableStyles.row(~m)} key=m##id>
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
          |> ReasonReact.array
        }
      </div>
    </UseRouterForLinks>;
  },
};