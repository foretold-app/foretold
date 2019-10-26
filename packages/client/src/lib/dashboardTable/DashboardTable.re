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
  let indexColumnType = (ts: ts, index) =>
    ts |> E.A.get(_, index) |> E.O.fmap(r => r.columnType);
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

module Json = {
  let decode = (j: Js.Json.t): Belt.Result.t(Table.t, string) => {
    let columnDecode = (json): Column.t =>
      Json.Decode.{
        id: json |> field("id", string),
        name: json |> field("name", string),
        columnType:
          switch (json |> field("columnType", string)) {
          | "MeasurableId" => MeasurableId
          | _ => String
          },
      };

    let columns = Json.Decode.(field("columns", array(columnDecode)));
    let cs = columns(j);

    // I don't think this will handle null values now :(
    let rowDecode = (columns: array(Column.t), json: Js.Json.t): Row.t =>
      columns
      |> E.A.fmap((column: Column.t) =>
           switch (column.columnType) {
           | String =>
             json
             |> Json.Decode.(optional(field(column.id, string)))
             |> E.O.fmap(r => String(r))
             |> E.O.default(Empty)
           | MeasurableId =>
             json
             |> Json.Decode.(optional(field(column.id, string)))
             |> E.O.fmap(r => MeasurableId(r))
             |> E.O.default(Empty)
           }
         );

    let rowsDecode = Json.Decode.(field("data", array(rowDecode(cs))));

    Belt.Result.Ok(Table.make(cs, rowsDecode(j)));
  };
};