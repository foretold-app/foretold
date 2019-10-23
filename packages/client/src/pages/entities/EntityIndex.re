open Utils;

let component = ReasonReact.statelessComponent("EntityShow");

module ColumnsFunctor = (Ken: KenTools.KenModule) => {
  type record = BsKen.Graph_T.T.thing;
  type column = Table.column(BsKen.Graph_T.T.thing);

  let nameColumn =
    Table.Column.make(
      ~name="Name" |> ste,
      ~render=
        (r: record) =>
          <Link.Jsx2
            linkType={Internal(EntityShow(r |> BsKen.Graph_T.Thing.id))}>
            {r |> Ken.getName |> ste}
          </Link.Jsx2>,
      ~flex=2,
      (),
    );

  let instanceOf =
    Table.Column.make(
      ~name="Instance Of" |> ste,
      ~render=(r: record) => r |> Ken.getInstanceOfName |> ste,
      (),
    );

  let idColumn =
    Table.Column.make(
      ~name="Name" |> ste,
      ~render=(r: record) => r |> BsKen.Graph_T.Thing.id |> ste,
      (),
    );

  let all = [|nameColumn, instanceOf, idColumn|];
};

let make = _children => {
  ...component,
  render: _ =>
    <Providers.AppContext.Consumer>
      ...{context => {
        module Config = {
          let globalSetting = context.globalSetting;
        };
        module Ken = KenTools.Functor(Config);
        module Columns = ColumnsFunctor(Ken);

        <SLayout head={SLayout.Header.textDiv("All Entities")}>
          {Table.fromColumns(Columns.all, Ken.dataSource, ())}
        </SLayout>;
      }}
    </Providers.AppContext.Consumer>,
};