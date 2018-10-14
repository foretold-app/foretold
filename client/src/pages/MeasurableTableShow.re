open Utils;
open Rationale;
open Queries;
open HandsOnTable;
open MeasurableColumns;
open Table;

let foo = str =>
  Rationale.Option.Infix.(
    ColumnBundle.make(
      ~headerName=str,
      ~get=
        e =>
          e.measurableTableAttributes
          >>= (r => Js.Dict.get(r, str))
          |> Option.default(""),
      (),
    )
  );

let bar = (ii: MetaTypeBase.measurableType) =>
  Array.map(((a, _)) => foo(a), ii.metaTypes);

let component = ReasonReact.statelessComponent("Measurables");
let make = (~id: string, _children) => {
  ...component,
  render: _self => {
    let item: option(MetaTypeBase.measurableType) =
      MetaTypeCollection.all
      |> Array.to_list
      |> Rationale.RList.find((e: MetaTypeBase.measurableType) => e.id == id);
    let xs =
      item |> Option.fmap(bar) |> Option.default([||]) |> Array.to_list;
    let transformations = [
      name,
      measurementCount,
      measurerCount,
      expectedResolutionDate,
      creator,
      isLocked,
      ...xs,
    ];
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
        Queries.GetMeasurablesQuery.make(o =>
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