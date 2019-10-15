[@bs.config {jsx: 3}];

module Columns = {
  type record = Types.channel;
  type column = Table.column(Types.channel);

  let nameDescriptionColumn =
    Table.Column.make(
      ~name="Name & Description" |> Utils.ste,
      ~render=
        (r: record) =>
          <div>
            <Link linkType={Internal(ChannelShow(r.id))}>
              [|r.name |> Utils.ste|]
            </Link>
            {r.description
             |> E.O.React.fmapOrNull(description =>
                  <Markdown.Jsx3 source=description />
                )}
          </div>,
      ~flex=4,
      (),
    );

  let memberCountColumn =
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

  let openedCountColumn =
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

  let labelsColumn =
    Table.Column.make(
      ~name="Curation" |> Utils.ste,
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
    nameDescriptionColumn,
    memberCountColumn,
    openedCountColumn,
    labelsColumn,
  |];
};

[@react.component]
let make = (~agentId=None, ~isArchived=?) =>
  ChannelsGet.component(
    ~channelMemberId=?agentId,
    ~isArchived?,
    ~sortFn=ChannelsGet.sortCurated,
    channels =>
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