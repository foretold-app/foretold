[@bs.config {jsx: 3}];

module Columns = {
  type record = Types.feedItem;
  type column = Table.column(Types.feedItem);

  let getName = (r: record): string =>
    r.body.common
    |> E.O.fmap((common: FeedItemBody.Common.t) => common.item)
    |> E.O.default("");

  let nameColumn: column =
    Table.Column.make(
      ~name="Item" |> Utils.ste,
      ~render=(r: record) => r |> getName |> Utils.ste,
      ~flex=2,
      ~show=(r: record) => r |> getName != "",
      (),
    );

  let descriptionColumn: column =
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

  let timeColumn: column =
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

  let all = [|nameColumn, descriptionColumn, timeColumn|];
};

[@react.component]
let make = (~feedItems) => Table.fromColumns(Columns.all, feedItems, ());

module Jsx2 = {
  let component = ReasonReact.statelessComponent("FeedItemsTable");
  let make = (~feedItems, children) =>
    ReasonReactCompat.wrapReactForReasonReact(
      make,
      makeProps(~feedItems, ()),
      children,
    );
};