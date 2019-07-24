[@bs.config {jsx: 3}];

module Columns = {
  type record = Primary.Channel.t;
  type column = Table.column(Primary.Channel.t);

  let nameColumn: column =
    Table.Column.make(
      ~name="Name" |> Utils.ste,
      ~render=
        (r: record) =>
          <Link linkType={Internal(ChannelShow(r.id))}>
            [|r.name |> Utils.ste|]
          </Link>,
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

[@react.component]
let make = (~loggedInUser: Primary.User.t, ~layout, ~children) =>
  ChannelsGet.component(channels =>
    SLayout.LayoutConfig.make(
      ~head=SLayout.Header.textDiv("Communities"),
      ~body=Table.fromColumns(Columns.all, channels, ()),
    )
    |> layout
  );

module Jsx2 = {
  let component = ReasonReact.statelessComponent("ChannelIndex");
  /* `children` is not labelled, as it is a regular parameter in version 2 of JSX */
  let make = (~loggedInUser, ~layout=SLayout.FullPage.makeWithEl, children) =>
    ReasonReactCompat.wrapReactForReasonReact(
      make,
      makeProps(~loggedInUser, ~layout, ~children, ()),
      children,
    );
};