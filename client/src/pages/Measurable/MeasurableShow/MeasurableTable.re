open Utils;
open Rationale;
open Rationale.Option.Infix;
open Rationale.Option;
open Rationale;
open Queries;
open HandsOnTable;
open MomentRe;
open MeasurableTypes;
open Table;

let component = ReasonReact.statelessComponent("MeasurableTable");

let toUnix = x => x##createdAt |> Moment.toUnix;

let victory = data => <SmallChart data />;

type tt = MeasurableTypes.measurement;
let foo = ColumnBundle.make(~headerName="Agent", ~get=(e: tt) => "sdf", ());

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
    ~headerName="Relevant at",
    ~get=(e: tt) => e##relevantAt |> Moment.format("L, h:mm:ss a"),
    (),
  ),
  ColumnBundle.make(
    ~headerName="Competitive",
    ~get=e => e##competitorType |> botCompetitor,
    (),
  ),
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
            (
              switch (floatCdf(e) <$> Value.FloatCdf.toPoints) {
              | Some(data) => <SmallChart data />
              | None => "" |> ste
              }
            )
          </div>,
        ),
    (),
  ),
];

let make = (~measurements: MeasurableTypes.measurements, _children) => {
  ...component,
  render: _ => {
    let data =
      measurements
      |> ArrayOptional.concatSomes
      |> Js_array.sortInPlaceWith((a, b) =>
           toUnix(b) > toUnix(a) ? (-1) : 1
         );
    Table.ColumnBundle.toHOT(~data, ~transformations);
  },
};