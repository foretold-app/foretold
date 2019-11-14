[@bs.config {jsx: 3}];

module Columns = {
  type record = Types.notebook;
  type column = Table.column(record);

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

  let time =
    Table.Column.make(
      ~name="Last Updated At" |> Utils.ste,
      ~render=
        (r: record) =>
          r.updatedAt
          |> E.O.fmap((updatedAt: MomentRe.Moment.t) =>
               updatedAt |> MomentRe.Moment.format("LLL")
             )
          |> E.O.default("")
          |> Utils.ste,
      ~flex=1,
      (),
    );

  let all = [|name, owner, time|];
};

[@react.component]
let make = (~items, ~columns=Columns.all, ~channelId: string) => {
  let onRowClb = (notebook: Types.notebook) => {
    Routing.Url.push(ChannelNotebook(channelId, notebook.id));
  };
  Table.fromColumns(columns, items, ~onRowClb=Some(onRowClb), ());
};