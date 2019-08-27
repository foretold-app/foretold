[@bs.config {jsx: 3}];

module Columns = {
  type record = Types.leaderboardItem;
  type column = Table.column(record);

  let getName = (r: record): ReasonReact.reactElement =>
    switch (r.measurable) {
    | Some(measurable) =>
      <Link
        linkType={
          Internal(MeasurableShow(measurable.channelId, measurable.id))
        }>
        [|measurable.name |> Utils.ste|]
      </Link>
    | _ => "" |> Utils.ste
    };

  let measurable =
    Table.Column.make(
      ~name="Question" |> Utils.ste,
      ~render=(r: record) => r |> getName,
      ~flex=2,
      (),
    );

  let agent =
    Table.Column.make(
      ~name="Agent" |> Utils.ste,
      ~render=
        (r: record) =>
          switch (r.agent) {
          | Some(agent) =>
            <Link
              linkType={
                Internal(
                  Agent({agentId: agent.id, subPage: AgentCommunities}),
                )
              }>
              [|
                agent.name
                |> E.O.fmap(name => name |> Utils.ste)
                |> E.O.default("Agent" |> Utils.ste),
              |]
            </Link>
          | _ => "" |> Utils.ste
          },
      ~flex=3,
      (),
    );

  let score =
    Table.Column.make(
      ~name="Score" |> Utils.ste,
      ~render=
        (r: record) =>
          switch (r.pointScore) {
          | Some(pointScore) => pointScore |> string_of_float |> Utils.ste
          | _ => "0.0" |> Utils.ste
          },
      ~flex=3,
      (),
    );

  let totalScore =
    Table.Column.make(
      ~name="Total Score" |> Utils.ste,
      ~render=
        (r: record) =>
          switch (r.pointScore) {
          | Some(pointScore) => pointScore |> Js.Float.toString |> Utils.ste
          | _ => "0.0" |> Utils.ste
          },
      ~flex=3,
      (),
    );

  let predictionCount =
    Table.Column.make(
      ~name="Prediction Count" |> Utils.ste,
      ~render=
        (r: record) =>
          switch (r.predictionCountTotal) {
          | Some(predictionCountTotal) =>
            predictionCountTotal |> string_of_int |> Utils.ste
          | _ => "0" |> Utils.ste
          },
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

  let default = [|measurable, agent, score, time|];
  let measurables = [|measurable, agent, totalScore, predictionCount, time|];
};

[@react.component]
let make = (~items, ~columns=Columns.default) =>
  Table.fromColumns(columns, items, ());

module Jsx2 = {
  let component = ReasonReact.statelessComponent("LeaderboardTable");
  let make = (~items, ~columns=Columns.default, children) =>
    ReasonReactCompat.wrapReactForReasonReact(
      make,
      makeProps(~items, ~columns, ()),
      children,
    );
};