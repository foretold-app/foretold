[@bs.config {jsx: 3}];

module Columns = {
  type record = Types.feedItem;
  type column = Table.column(Types.feedItem);

  let getName = (r: record): string =>
    r.body.common
    |> E.O.fmap((common: FeedItemBody.Common.t) => common.item)
    |> E.O.default("");

  let channel: column =
    Table.Column.make(
      ~name="Channel" |> Utils.ste,
      ~render=
        (r: record) =>
          <Link linkType={Internal(ChannelShow(r.channelId))}>
            [|r.channel.name |> Utils.ste|]
          </Link>,
      ~flex=2,
      (),
    );

  let item: column =
    Table.Column.make(
      ~name="Item" |> Utils.ste,
      ~render=(r: record) => r |> getName |> Utils.ste,
      ~flex=2,
      ~show=(r: record) => r |> getName != "",
      (),
    );

  let description: column =
    Table.Column.make(
      ~name="Description" |> Utils.ste,
      ~render=
        (r: record) =>
          r.body.common
          |> E.O.fmap((common: FeedItemBody.Common.t) => common.description)
          |> E.O.default("")
          |> Utils.ste,
      ~flex=3,
      (),
    );

  let time: column =
    Table.Column.make(
      ~name="Time" |> Utils.ste,
      ~render=
        (r: record) =>
          r.createdAt
          |> E.O.fmap((createdAt: MomentRe.Moment.t) =>
               createdAt |> MomentRe.Moment.format("LLL")
             )
          |> E.O.default("")
          |> Utils.ste,
      ~flex=1,
      (),
    );

  let all = [|channel, item, description, time|];
  let short = [|item, description, time|];
};

[@react.component]
let make = (~feedItems, ~columns=Columns.all) =>
  Table.fromColumns(columns, feedItems, ());

module Jsx2 = {
  let component = ReasonReact.statelessComponent("FeedItemsTable");
  let make = (~feedItems, ~columns=Columns.all, children) =>
    ReasonReactCompat.wrapReactForReasonReact(
      make,
      makeProps(~feedItems, ~columns, ()),
      children,
    );
};