open Utils;

let component = ReasonReact.statelessComponent("EntityShow");

module Columns = {
  type record = EKen.Thing.t;
  type column = Table.column(EKen.Thing.t);

  let nameColumn: column =
    Table.Column.make(
      ~name="Name" |> ste,
      ~render=
        (r: record) =>
          <Link.Jsx2 linkType={Internal(EntityShow(r |> Graph_T.Thing.id))}>
            {r |> EKen.Thing.getName |> ste}
          </Link.Jsx2>,
      ~flex=2,
      (),
    );

  let instanceOf: column =
    Table.Column.make(
      ~name="Instance Of" |> ste,
      ~render=(r: record) => r |> EKen.Thing.getInstanceOfName |> ste,
      (),
    );

  let idColumn: column =
    Table.Column.make(
      ~name="Name" |> ste,
      ~render=(r: record) => r |> Graph_T.Thing.id |> ste,
      (),
    );

  let all = [|nameColumn, instanceOf, idColumn|];
};

let dataSource = EKen.Things.getAll |> EKen.Things.withNames;

let make = (~layout=SLayout.FullPage.makeWithEl, _children) => {
  ...component,
  render: _ =>
    SLayout.LayoutConfig.make(
      ~head=SLayout.Header.textDiv("All Entities"),
      ~body=Table.fromColumns(Columns.all, dataSource, ()),
    )
    |> layout,
};