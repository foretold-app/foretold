let component = ReasonReact.statelessComponent("DashboardTable");

let template =
  DashboardTable.Table.make(
    [|
      {id: "1", name: "Name", columnType: String},
      {id: "2", name: "Description", columnType: String},
    |],
    [|
      [|String("Canada"), String("Country 1")|],
      [|String("US"), String("Country 2")|],
    |],
  );

let foo = [3, 4, 5];
let foobar = [|3, 4, 5|];

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
                | Some(measurable) => "GOT IT" |> Utils.ste
                | None => "Nothing :(" |> Utils.ste
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

let make = (~table: DashboardTable.Table.t=template, _) => {
  ...component,
  render: _ =>
    MeasurablesGet.component(
      ~channelId=Some("sdf"),
      ~states=[|Some(`OPEN)|],
      ~pageLimit=Js.Json.number(50 |> float_of_int),
      ~direction=None,
      ~innerComponentFn=
        e =>
          e
          |> HttpResponse.fmap(
               (r: Client.Primary.Connection.t(Client.Primary.Measurable.t)) =>
               DashboardTableToTable.run(table, r.edges)
             )
          |> HttpResponse.withReactDefaults,
      (),
    ),
};