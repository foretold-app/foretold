[@bs.config {jsx: 3}];

module DashboardTableToTable = {
  let toColumn =
      (
        measurables: array(Primary.Measurable.t),
        editor: DashboardTableEditor.editor,
        index: int,
        column: DashboardTable.Column.t,
      )
      : Table.column(DashboardTable.Row.t) =>
    Table.Column.make(
      ~name=column.name |> Utils.ste,
      ~render=
        (c: DashboardTable.Row.t) =>
          switch (Belt.Array.get(c, index)) {
          | Some(String(str)) => str |> Utils.ste
          | Some(MeasurementValue(_)) => "MeasurementValue" |> Utils.ste
          | Some(MeasurableId(str)) =>
            measurables
            |> E.A.getBy(_, r => r.id == str)
            |> (
              r =>
                switch (r) {
                | Some(measurable) =>
                  <div
                    onClick={_ => editor.onSelect(measurable.id)}
                    className=Css.(
                      style([
                        borderRadius(`px(4)),
                        marginLeft(`px(3)),
                        padding(`px(5)),
                        backgroundColor(
                          Some(measurable.id) == editor.selectedId
                            ? `hex("eff3f5") : `hex("fff"),
                        ),
                        hover([
                          backgroundColor(`hex("eee")),
                          cursor(`pointer),
                        ]),
                      ])
                    )>
                    <MeasurementItems.AggregationResolution measurable />
                    <MeasurementItems.AgentMeasurement measurable />
                  </div>
                | None =>
                  <FC__Alert type_=`warning>
                    {"Not loaded" |> Utils.ste}
                  </FC__Alert>
                }
            )
          | Some(Empty)
          | None => "" |> Utils.ste
          },
      (),
    );

  let run =
      (
        table: DashboardTable.Table.t,
        measurables: array(Primary.Measurable.t),
        editor: DashboardTableEditor.editor,
      ) => {
    let columns =
      table.columns
      |> Belt.Array.mapWithIndex(_, toColumn(measurables, editor));
    Table.fromColumns(columns, table.rows, ());
  };
};

let tableJsonString = {| { "columns": [{"id":"1", "name": "Name", "columnType": "String"},{"id":"2", "name": "Description", "columnType": "String"}], "data": [{"1": "Thing1", "2":"This is a long description"},{"1":"Thing2"}] } |};
let tableJson: Js.Json.t = Json.parseOrRaise(tableJsonString);

[@react.component]
let make = (~tableJson=tableJson, ~editor: DashboardTableEditor.editor) => {
  let tableJson = React.useMemo(_ => DashboardTable.Json.decode(tableJson));
  switch (tableJson) {
  | Ok(table) =>
    MeasurablesGet.component(
      ~measurableIds=Some(DashboardTable.Table.allMeasurableIds(table)),
      ~pageLimit=Js.Json.number(500 |> float_of_int),
      ~direction=None,
      ~innerComponentFn=
        e =>
          e
          |> HttpResponse.fmap(
               (r: Client.Primary.Connection.t(Client.Primary.Measurable.t)) =>
               DashboardTableToTable.run(table, r.edges, editor)
             )
          |> HttpResponse.withReactDefaults,
      (),
    )
  | Error(e) => e |> Utils.ste
  };
};