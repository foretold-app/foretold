open Utils;
open Rationale;
open Queries;
open HandsOnTable;
open MeasurableColumns;
open Table;
open MetaTypeBase;

let schemaTypeColumn = (id, name) =>
  Rationale.Option.Infix.(
    ColumnBundle.make(
      ~headerName=name,
      ~get=
        e =>
          e.measurableTableAttributes
          >>= (r => Js.Dict.get(r, id))
          |> Option.default(""),
      (),
    )
  );

let generateSchemaTypeColumn = (ii: MetaTypeBase.measurableType) =>
  Belt.List.map(ii.schema, r => schemaTypeColumn(r.id, r.name));

let component = ReasonReact.statelessComponent("Measurables");
let ys = [
  measurementCount,
  measurerCount,
  expectedResolutionDate,
  creator,
  isLocked,
];
let make = (~id: string, _children) => {
  ...component,
  render: _self => {
    let item: option(MetaTypeBase.measurableType) =
      MetaTypeItems.all
      |> Array.to_list
      |> Rationale.RList.find((e: MetaTypeBase.measurableType) => e.id == id);
    let xs =
      item |> Option.fmap(generateSchemaTypeColumn) |> Option.default([]);
    let transformations =
      List.concat([[nameColumn, descriptionColumn], xs, ys]);
    let query = Queries.GetMeasurables2.make(~measurableTableId=id, ());
    <div>
      <div>
        (
          item
          |> Option.fmap((e: MetaTypeBase.measurableType) => e.name)
          |> Option.default("")
          |> ste
        )
      </div>
      Result.Infix.(
        Queries.GetMeasurablesQuery2.make(~variables=query##variables, o =>
          o.result
          |> apolloResponseToResult
          <$> (d => d##measurables)
          <$> Extensions.Array.concatSomes
          <$> Table.ColumnBundle.toHOT(~data=_, ~transformations, ())
          |> Result.result(idd, idd)
        )
        |> ReasonReact.element
      )
    </div>
    |> NormalLayout.make(~name="Measurables")
    |> ReasonReact.element;
  },
};