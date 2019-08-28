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
      ~flex=4,
      (),
    );

  let agent =
    Table.Column.make(
      ~name="Member" |> Utils.ste,
      ~render=
        (r: record) =>
          switch (r.agent) {
          | Some(agent) =>
            <Link
              linkType={
                Internal(Agent({agentId: agent.id, subPage: AgentUpdates}))
              }>
              [|
                agent.name
                |> E.O.fmap(name => name |> Utils.ste)
                |> E.O.default("Member" |> Utils.ste),
              |]
            </Link>
          | _ => "" |> Utils.ste
          },
      ~flex=1,
      (),
    );

  let score =
    Table.Column.make(
      ~name="Score" |> Utils.ste,
      ~render=
        (r: record) =>
          switch (r.pointScore) {
          | Some(pointScore) => pointScore |> E.Float.toString |> Utils.ste
          | _ => "0.0" |> Utils.ste
          },
      ~flex=1,
      (),
    );

  let totalScore =
    Table.Column.make(
      ~name="Total Score" |> Utils.ste,
      ~render=
        (r: record) =>
          switch (r.pointScore) {
          | Some(pointScore) => pointScore |> E.Float.toString |> Utils.ste
          | _ => "0.0" |> Utils.ste
          },
      ~flex=1,
      (),
    );

  let predictionCount =
    Table.Column.make(
      ~name="Predictions" |> Utils.ste,
      ~render=
        (r: record) =>
          switch (r.predictionCountTotal) {
          | Some(predictionCountTotal) =>
            predictionCountTotal |> Js.Int.toString |> Utils.ste
          | _ => "" |> Utils.ste
          },
      ~flex=1,
      (),
    );

  let predictedMeasurablesCount =
    Table.Column.make(
      ~name="Predicted Questions" |> Utils.ste,
      ~render=
        (r: record) =>
          switch (r.numberOfQuestionsScored) {
          | Some(numberOfQuestionsScored) =>
            numberOfQuestionsScored |> Js.Int.toString |> Utils.ste
          | _ => "0" |> Utils.ste
          },
      ~flex=1,
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

  let default = [|agent, measurable, score, time|];
  let measurables = [|agent, measurable, totalScore, predictionCount, time|];
  let members = [|
    agent,
    totalScore,
    predictedMeasurablesCount,
    predictionCount,
  |];
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