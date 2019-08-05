[@bs.config {jsx: 3}];

module Columns = {
  type record = Types.feedItem;
  type column = Table.column(Types.feedItem);

  let nameColumn: column =
    Table.Column.make(
      ~name="Item" |> Utils.ste,
      ~render=
        (r: record) =>
          r.body.common
          |> E.O.fmap((common: FeedItemBody.Common.t) => common.item)
          |> E.O.default("")
          |> Utils.ste,
      ~flex=2,
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

  let all = [|nameColumn, descriptionColumn|];
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