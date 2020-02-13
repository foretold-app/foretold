type domain = {
  xMin: option(float),
  xMax: option(float),
};

type columnType =
  | String
  | MeasurableId(domain)
  | MeasurementValue;

module Column = {
  type t = {
    id: string,
    name: string,
    width: int,
    columnType,
  };
  type ts = array(t);

  let indexColumnType = (ts: ts, index) =>
    ts |> E.A.get(_, index) |> E.O.fmap(r => r.columnType);

  let domain = (t: t) =>
    switch (t.columnType) {
    | MeasurableId(d) => d
    | _ => {xMin: None, xMax: None}
    };
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
         | MeasurableId(measurableId) => Some(measurableId)
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
    t.rows |> E.A.fmap(Row.measurableIds) |> E.A.uniq |> E.A.concatMany;
};

module Json = {
  let decode = (j: Js.Json.t): Belt.Result.t(Table.t, string) => {
    let decodeField = (tt, fieldName, json) =>
      Json.Decode.(json |> optional(field(fieldName, tt)));

    let columnDecode = (json): Belt.Result.t(Column.t, string) =>
      switch (
        decodeField(Json.Decode.string, "id", json),
        decodeField(Json.Decode.string, "name", json),
        decodeField(Json.Decode.string, "columnType", json),
      ) {
      | (Some(id), Some(name), Some(columnType)) =>
        Belt.Result.Ok({
          id,
          name,
          width:
            decodeField(Json.Decode.int, "width", json) |> E.O.default(1),
          columnType:
            switch (columnType) {
            | "MeasurableId" =>
              MeasurableId({
                xMin: decodeField(Json.Decode.float, "xMin", json),
                xMax: decodeField(Json.Decode.float, "xMax", json),
              })
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
           | MeasurableId(_) =>
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