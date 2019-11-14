[@bs.config {jsx: 3}];

type columnType =
  | String
  | MeasurableId
  | MeasurementValue;

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
  | MeasurementValue(MeasurementValue.t)
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
    let decodeField = (fieldName, json) =>
      Json.Decode.(json |> optional(field(fieldName, string)));

    let columnDecode = (json): Belt.Result.t(Column.t, string) =>
      switch (
        decodeField("id", json),
        decodeField("name", json),
        decodeField("columnType", json),
      ) {
      | (Some(id), Some(name), Some(columnType)) =>
        Belt.Result.Ok({
          id,
          name,
          columnType:
            switch (columnType) {
            | "MeasurableId" => MeasurableId
            | "MeasurementValue" => MeasurementValue
            | _ => String
            },
        })
      | _ => Belt.Result.Error("All columns need id, name, and columnType")
      };

    let columns = json =>
      json
      |> Json.Decode.(optional(field("columns", array(columnDecode))))
      |> E.O.fmap(E.A.R.firstErrorOrOpen)
      |> {
        e =>
          switch (e) {
          | Some(result) => result
          | None => Belt.Result.Error("Need at least one row.")
          };
      };

    let rowDecode = (columns: Column.ts, json: Js.Json.t): Row.t =>
      columns
      |> E.A.fmap((column: Column.t) =>
           switch (column.columnType) {
           | String =>
             json
             |> Json.Decode.(optional(field(column.id, string)))
             |> E.O.fmap(r => String(r))
             |> E.O.default(Empty)
           | MeasurementValue =>
             switch (
               json
               |> Json.Decode.(
                    optional(field(column.id, MeasurementValue.decode))
                  )
             ) {
             | Some(Ok(r)) => MeasurementValue(r)
             | Some(Error(_)) => Empty
             | _ => Empty
             }
           | MeasurableId =>
             json
             |> Json.Decode.(optional(field(column.id, string)))
             |> E.O.fmap(r => MeasurableId(r))
             |> E.O.default(Empty)
           }
         );

    let rowsDecode = columns =>
      Json.Decode.(optional(field("data", array(rowDecode(columns)))));

    let columns = columns(j);
    switch (columns) {
    | Belt.Result.Ok(columns) =>
      switch (rowsDecode(columns, j)) {
      | Some(rows) => Belt.Result.Ok(Table.make(columns, rows))
      | None =>
        Belt.Result.Error(
          "There needs to be a data field with at least 1 valid row.",
        )
      }
    | Belt.Result.Error(error) => Belt.Result.Error(error)
    };
  };
};