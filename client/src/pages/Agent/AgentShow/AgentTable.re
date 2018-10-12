open Utils;
open Rationale;
open Rationale.Option.Infix;
open Rationale.Option;
open HandsOnTable;
open MomentRe;
open AgentTypes;
open Table;

let toMeasurableLink = (m: AgentTypes.measurable) => {
  let id = m.id;
  let name = m.name;
  {j|<a href="/measurables/$id">$name</a>|j};
};

let createdAt =
  ColumnBundle.make(
    ~headerName="Created At",
    ~get=e => e.createdAt |> Moment.format("L, h:mm:ss a"),
    (),
  );

let value =
  ColumnBundle.make(
    ~headerName="Created At",
    ~get=e => Belt.Result.mapWithDefault(e.value, "", Value.stringOfValue),
    (),
  );

let measurable =
  ColumnBundle.make(
    ~headerName="Measurable",
    ~get=e => e.measurable <$> toMeasurableLink |> Option.default(""),
    ~column=Columns.html,
    (),
  );

let component = ReasonReact.statelessComponent("MeasurableTable");
let make = (~measurements: array(AgentTypes.measurement), _children) => {
  ...component,
  render: _ =>
    Table.ColumnBundle.toHOT(
      ~data=measurements,
      ~transformations=[createdAt, value, measurable],
      (),
    ),
};