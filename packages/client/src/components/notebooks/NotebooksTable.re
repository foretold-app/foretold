module Columns = {
  type record = Types.notebook;
  type column = Table.column(record);

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

  let name =
    Table.Column.make(
      ~name="Name" |> Utils.ste,
      ~render=(r: record) => r.name |> Utils.ste,
      ~flex=3,
      (),
    );

  let owner =
    Table.Column.make(
      ~name="Owner" |> Utils.ste,
      ~render=(r: record) => <AgentLink agent={r.owner} />,
      ~flex=2,
      (),
    );

  let bookmarksCount =
    Table.Column.make(
      ~name="Bookmark Count" |> Utils.ste,
      ~render=
        (notebook: record) =>
          notebook.bookmarksCount
          |> E.O.fmap(string_of_int)
          |> E.O.default("")
          |> Utils.ste,
      (),
    );

  let time =
    Table.Column.make(
      ~name="Last Updated" |> Utils.ste,
      ~render=
        (r: record) =>
          r.updatedAt
          |> E.O.fmap((updatedAt: MomentRe.Moment.t) =>
               updatedAt |> MomentRe.Moment.fromNow(~withoutSuffix=None)
             )
          |> E.O.default("")
          |> Utils.ste,
      (),
    );

  let all = [|name, channel, owner, bookmarksCount, time|];
  let short = [|name, owner, bookmarksCount, time|];
};

[@react.component]
let make = (~items, ~columns=Columns.all) => {
  let onRowClb = (notebook: Types.notebook) => {
    Routing.Url.push(ChannelNotebook(notebook.channelId, notebook.id));
  };

  <Table columns rows=items onRowClb={Some(onRowClb)} />;
};