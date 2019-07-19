open Css;

let component = ReasonReact.statelessComponent("ChannelIndex");

let row =
  style([
    width(`percent(100.0)),
    borderBottom(`px(1), `solid, hex("eee")),
    display(`flex),
    flexDirection(`row),
    paddingLeft(`px(8)),
    paddingRight(`px(8)),
    paddingTop(`px(8)),
    paddingBottom(`px(7)),
    cursor(`pointer),
    backgroundColor(`hex("fff")),
    selector(":last-child", [borderBottom(`px(0), `solid, hex("eee"))]),
    selector(":hover", [backgroundColor(`hex("eef0f3"))]),
  ]);

let column = style([flex(1)]);

let table =
  style([
    overflowY(`auto),
    borderRadius(`px(2)),
    border(`px(1), `solid, `hex("ddd")),
    marginTop(`em(2.)),
    marginBottom(`em(2.)),
  ]);

let nameS = style([fontWeight(`black), fontSize(`em(1.2))]);

module Columns = {
  type record = Primary.Channel.t;
  type column = Table.column(Primary.Channel.t);

  let nameColumn: column =
    Table.Column.make(
      ~name="Name" |> Utils.ste,
      ~render=
        (r: record) =>
          <Foretold__Components__Link linkType={Internal(ChannelShow(r.id))}>
            {r.name |> Utils.ste}
          </Foretold__Components__Link>,
      ~flex=2,
      (),
    );

  let descriptionColumn: column =
    Table.Column.make(
      ~name="Description" |> Utils.ste,
      ~render=(r: record) => r.description |> E.O.default("") |> Utils.ste,
      ~flex=3,
      (),
    );

  let memberCountColumn: column =
    Table.Column.make(
      ~name="Members" |> Utils.ste,
      ~render=
        (r: record) =>
          r.membershipCount
          |> E.O.fmap(string_of_int)
          |> E.O.default("")
          |> Utils.ste,
      ~flex=1,
      (),
    );

  let openedCountColumn: column =
    Table.Column.make(
      ~name="Open Questions" |> Utils.ste,
      ~render=
        (r: record) =>
          r.openedMeasurablesCount
          |> E.O.fmap(string_of_int)
          |> E.O.default("")
          |> Utils.ste,
      ~flex=1,
      (),
    );

  let curatedColumn: column =
    Table.Column.make(
      ~name="Labels" |> Utils.ste,
      ~render=
        (r: record) =>
          r.isCurated
            ? <div className="ant-tag ant-tag-blue">
                {"Curated" |> ReasonReact.string}
              </div>
            : ReasonReact.null,
      ~show=(r: record) => r.isCurated,
      ~flex=1,
      (),
    );

  let all = [|
    nameColumn,
    descriptionColumn,
    memberCountColumn,
    openedCountColumn,
    curatedColumn,
  |];
};

let make =
    (
      ~loggedInUser: Primary.User.t,
      ~layout=SLayout.FullPage.makeWithEl,
      _children,
    ) => {
  ...component,
  render: _ =>
    ChannelsGet.component(channels =>
      SLayout.LayoutConfig.make(
        ~head=SLayout.Header.textDiv("Communities"),
        ~body=Table.fromColumns(Columns.all, channels, ()),
      )
      |> layout
    ),
};