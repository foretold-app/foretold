[@bs.config {jsx: 3}];

module DashboardTableToTable = {
  let toColumn =
      (
        measurables: array(Primary.Measurable.t),
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
          | Some(MeasurableId(str)) =>
            measurables
            |> E.A.getBy(_, r => r.id == str)
            |> (
              r =>
                switch (r) {
                | Some(measurable) =>
                  <div>
                    <MeasurementItems.AggregationResolution measurable />
                    <Link.Jsx2
                      className=Shared.Item.item
                      linkType={
                        Internal(
                          MeasurableShow(measurable.channelId, measurable.id),
                        )
                      }>
                      {"Link" |> Utils.ste}
                    </Link.Jsx2>
                  </div>
                | None =>
                  <FC__Alert type_=`warning>
                    {"Not loaded :(" |> Utils.ste}
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
      ) => {
    let columns =
      table.columns |> Belt.Array.mapWithIndex(_, toColumn(measurables));
    Table.fromColumns(columns, table.rows, ());
  };
};

let tableJsonString = {| { "columns": [{"id":"1", "name": "Name", "columnType": "String"},{"id":"2", "name": "Description", "columnType": "String"}], "data": [{"1": "Thing1", "2":"This is a long description"},{"1":"Thing2"}] } |};
let tableJson: Js.Json.t = Json.parseOrRaise(tableJsonString);

[@react.component]
let make = (~channelId, ~tableJson=tableJson, _) => {
  MeasurablesGet.component(
    ~channelId=Some(channelId),
    ~states=[|Some(`OPEN)|],
    ~pageLimit=Js.Json.number(100 |> float_of_int),
    ~direction=None,
    ~pollInterval=20 * 1000,
    ~innerComponentFn=
      e =>
        e
        |> HttpResponse.fmap(
             (r: Client.Primary.Connection.t(Client.Primary.Measurable.t)) =>
             switch (DashboardTable.Json.decode(tableJson)) {
             | Ok(table) => DashboardTableToTable.run(table, r.edges)
             | Error(e) => e |> Utils.ste
             }
           )
        |> HttpResponse.withReactDefaults,
    (),
  );
};
