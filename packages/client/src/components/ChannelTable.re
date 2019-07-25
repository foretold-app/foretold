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

  let labelsColumn: column =
    Table.Column.make(
      ~name="Labels" |> Utils.ste,
      ~render=
        (r: record) =>
          <>
            {r.isCurated
               ? <div className="ant-tag ant-tag-blue">
                   {"Curated" |> ReasonReact.string}
                 </div>
               : ReasonReact.null}
            {r.isArchived
               ? <div className="ant-tag ant-tag-gold">
                   {"Archived" |> ReasonReact.string}
                 </div>
               : ReasonReact.null}
          </>,
      ~show=(r: record) => r.isCurated || r.isArchived,
      ~flex=1,
      (),
    );

  let all = [|
    nameColumn,
    descriptionColumn,
    memberCountColumn,
    openedCountColumn,
    labelsColumn,
  |];
};

[@react.component]
let make = (~agentId=None, ~isArchived=?) =>
  ChannelsGet.component(~channelMemberId=?agentId, ~isArchived?, channels =>
    Table.fromColumns(Columns.all, channels, ())
  );

module Jsx2 = {
  let component = ReasonReact.statelessComponent("ChannelTable");
  let make = (~agentId=None, ~isArchived=?, children) =>
    ReasonReactCompat.wrapReactForReasonReact(
      make,
      makeProps(~agentId, ~isArchived?, ()),
      children,
    );
};