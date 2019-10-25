type columnType =
  | String
  | MeasurableId;

module Column = {
  type t = {
    id: string,
    name: string,
    columnType,
  };
  type ts = array(t);
};

type cell =
  | String(string)
  | MeasurableId(string)
  | Empty;

module Row = {
  type t = array(cell);
  type ts = array(t);
  let removeInvalidCells = (columns: Column.ts, row: t) => row;
  let measurableIds = (t: t) =>
    t
    |> E.A.fmap(r =>
         switch (r) {
         | MeasurableId(id) => Some(id)
         | _ => None
         }
       )
    |> E.A.O.concatSome;
};

module Table = {
  type t = {
    columns: Column.ts,
    rows: Row.ts,
  };
  let make = (columns, rows) => {columns, rows};
  let allMeasurableIds = (t: t) =>
    t.rows |> E.A.fmap(Row.measurableIds) |> E.A.concatMany;
};

let rows: Row.ts = [|[|String("sdf")|]|];