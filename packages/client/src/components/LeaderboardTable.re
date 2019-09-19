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
        [|measurable.name |> E.S.default("Question") |> Utils.ste|]
      </Link>
    | _ => "Question" |> Utils.ste
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
          r.agent
          |> E.O.fmap((agent: Types.agent) =>
               <Link
                 linkType={
                   Internal(
                     Agent({agentId: agent.id, subPage: AgentUpdates}),
                   )
                 }>
                 [|
                   agent.name
                   |> E.O.fmap(E.S.default("Member"))
                   |> E.O.default("Member")
                   |> Utils.ste,
                 |]
               </Link>
             )
          |> E.O.default("Member" |> Utils.ste),
      ~flex=1,
      (),
    );

  let score =
    Table.Column.make(
      ~name="Score" |> Utils.ste,
      ~render=
        (r: record) =>
          r.pointScore
          |> E.O.fmap(E.Float.toString)
          |> E.O.default("")
          |> Utils.ste,
      ~flex=1,
      (),
    );

  let totalScore =
    Table.Column.make(
      ~name="Total Score" |> Utils.ste,
      ~render=
        (r: record) =>
          r.pointScore
          |> E.O.fmap(E.Float.toString)
          |> E.O.default("")
          |> Utils.ste,
      ~flex=1,
      (),
    );

  let predictionCount =
    Table.Column.make(
      ~name="Predictions" |> Utils.ste,
      ~render=
        (r: record) =>
          r.predictionCountTotal
          |> E.O.fmap(E.I.toString)
          |> E.O.default("")
          |> Utils.ste,
      ~flex=1,
      (),
    );

  let predictedMeasurablesCount =
    Table.Column.make(
      ~name="Predicted Questions" |> Utils.ste,
      ~render=
        (r: record) =>
          r.numberOfQuestionsScored
          |> E.O.fmap(E.I.toString)
          |> E.O.default("")
          |> Utils.ste,
      ~flex=1,
      (),
    );

  let time =
    Table.Column.make(
      ~name="Time" |> Utils.ste,
      ~render=
        (r: record) =>
          r.createdAt
          |> E.O.fmap(MomentRe.Moment.format("LLL"))
          |> E.O.default("")
          |> Utils.ste,
      ~flex=1,
      (),
    );

  let getMeasurement = measurement => {
    let bounds = MeasurementsTable.Helpers.bounds([|measurement|]);
    MeasurementsTable.Helpers.smallDistribution(
      ~measurement,
      ~bounds,
      ~width=75,
      ~height=30,
      (),
    )
    |> E.O.default("" |> Utils.ste);
  };

  let competitiveMeasurement =
    Table.Column.make(
      ~name="Prediction" |> Utils.ste,
      ~render=
        (r: record) =>
          r.competitiveMeasurement
          |> E.O.fmap(getMeasurement)
          |> E.O.default("" |> Utils.ste),
      ~flex=1,
      (),
    );

  let aggregationMeasurement =
    Table.Column.make(
      ~name="Recent Aggregate" |> Utils.ste,
      ~render=
        (r: record) =>
          r.aggregationMeasurement
          |> E.O.fmap(getMeasurement)
          |> E.O.default("" |> Utils.ste),
      ~flex=1,
      (),
    );

  let objectiveMeasurement =
    Table.Column.make(
      ~name="Final Answer" |> Utils.ste,
      ~render=
        (r: record) =>
          r.objectiveMeasurement
          |> E.O.fmap(getMeasurement)
          |> E.O.default("" |> Utils.ste),
      ~flex=1,
      (),
    );

  let default = [|
    agent,
    measurable,
    competitiveMeasurement,
    aggregationMeasurement,
    objectiveMeasurement,
    score,
    time,
  |];

  let measurables = [|
    agent,
    measurable,
    objectiveMeasurement,
    totalScore,
    predictionCount,
    time,
  |];

  let measurables' = [|
    agent,
    objectiveMeasurement,
    totalScore,
    predictionCount,
    time,
  |];

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