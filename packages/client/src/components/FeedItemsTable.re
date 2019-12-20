module Columns = {
  type record = Types.feedItem;
  type column = Table.column(record);

  let getName = (r: record): ReasonReact.reactElement =>
    switch (r.body.generic, r.body.measurable) {
    | (Some(row), _) => row.item |> Utils.ste
    | (_, Some(row)) =>
      <Link
        linkType={Internal(MeasurableShow(r.channelId, row.measurableId))}>
        {row.item |> Utils.ste}
      </Link>
    | _ => "" |> Utils.ste
    };

  let getDescription = (r: record): string =>
    switch (r.body.generic, r.body.measurable) {
    | (Some(row), _) => row.description
    | (_, Some(row)) => row.description
    | _ => ""
    };

  let channel =
    Table.Column.make(
      ~name="Community" |> Utils.ste,
      ~render=
        (r: record) =>
          <Link linkType={Internal(ChannelShow(r.channelId))}>
            {r.channel.name |> Utils.ste}
          </Link>,
      ~flex=2,
      (),
    );

  let item =
    Table.Column.make(
      ~name="Question" |> Utils.ste,
      ~render=(r: record) => r |> getName,
      ~flex=3,
      (),
    );

  let description =
    Table.Column.make(
      ~name="Description" |> Utils.ste,
      ~render=(r: record) => r |> getDescription |> Utils.ste,
      ~flex=3,
      (),
    );

  let time =
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