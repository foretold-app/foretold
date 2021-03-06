module Columns = {
  type record = Types.channel;
  type column = Table.column(Types.channel);

  let nameDescription =
    Table.Column.make(
      ~name="Name & Description" |> Utils.ste,
      ~render=
        (channel: record) =>
          <div>
            <Link linkType={Internal(ChannelShow(channel.id))}>
              {channel.name |> Utils.ste}
            </Link>
            <ChannelDescription channel />
          </div>,
      ~flex=4,
      (),
    );

  let memberCount =
    Table.Column.make(
      ~name="Members" |> Utils.ste,
      ~render=
        (channel: record) =>
          channel.membershipCount
          |> E.O.fmap(string_of_int)
          |> E.O.default("")
          |> Utils.ste,
      (),
    );

  let openedCount =
    Table.Column.make(
      ~name="Open Questions" |> Utils.ste,
      ~render=
        (channel: record) =>
          channel.openedMeasurablesCount
          |> E.O.fmap(string_of_int)
          |> E.O.default("")
          |> Utils.ste,
      (),
    );

  let bookmarksCount =
    Table.Column.make(
      ~name="Bookmark Count" |> Utils.ste,
      ~render=
        (channel: record) =>
          channel.bookmarksCount
          |> E.O.fmap(string_of_int)
          |> E.O.default("")
          |> Utils.ste,
      (),
    );

  let labels =
    Table.Column.make(
      ~name="Curation" |> Utils.ste,
      ~render=
        (channel: record) =>
          <> <Curated channel /> <IsArchived channel /> </>,
      ~show=(channel: record) => channel.isCurated || channel.isArchived,
      (),
    );

  let all = [|
    nameDescription,
    memberCount,
    openedCount,
    bookmarksCount,
    labels,
  |];
};

[@react.component]
let make = (~agentId=None, ~isArchived=?) =>
  ChannelsGet.component(
    ~channelMemberId=?agentId,
    ~order=ChannelsGet.orderChannels,
    ~isArchived?,
    channels =>
    <Table columns=Columns.all rows=channels />
  );