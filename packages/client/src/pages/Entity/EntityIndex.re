open Utils;

let component = ReasonReact.statelessComponent("EntityShow");

module ColumnsFunctor = (Ken: KenTools.KenModule) => {
  type record = Graph_T.T.thing;
  type column = Table.column(Graph_T.T.thing);

  let nameColumn: column =
    Table.Column.make(
      ~name="Name" |> ste,
      ~render=
        (r: record) =>
          <Link.Jsx2 linkType={Internal(EntityShow(r |> Graph_T.Thing.id))}>
            {r |> Ken.getName |> ste}
          </Link.Jsx2>,
      ~flex=2,
      (),
    );

  let instanceOf: column =
    Table.Column.make(
      ~name="Instance Of" |> ste,
      ~render=(r: record) => r |> Ken.getInstanceOfName |> ste,
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

let make = (~layout=SLayout.FullPage.makeWithEl, _children) => {
  ...component,
  render: _ =>
    <Providers.AppContext.Consumer>
      ...{context => {
        module Config = {
          let globalSetting = context.globalSetting;
        };
        module Ken = KenTools.Functor(Config);
        module Columns = ColumnsFunctor(Ken);

        SLayout.LayoutConfig.make(
          ~head=SLayout.Header.textDiv("All Entities"),
          ~body=Table.fromColumns(Columns.all, Ken.dataSource, ()),
        )
        |> layout;
      }}
    </Providers.AppContext.Consumer>,
};