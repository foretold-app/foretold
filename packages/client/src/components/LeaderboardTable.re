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
      ~name="Agent" |> Utils.ste,
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
                   |> E.O.fmap(E.S.default("Agent"))
                   |> E.O.default("Agent")
                   |> Utils.ste,
                 |]
               </Link>
             )
          |> E.O.default("Agent" |> Utils.ste),
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
          |> E.O.default("0.0")
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
          |> E.O.default("0.0")
          |> Utils.ste,
      ~flex=1,
      (),
    );

  let predictionCount =
    Table.Column.make(
      ~name="Prediction Count" |> Utils.ste,
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
      ~name="Predicted Question Count" |> Utils.ste,
      ~render=
        (r: record) =>
          r.numberOfQuestionsScored
          |> E.O.fmap(E.I.toString)
          |> E.O.default("0")
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
    let bounds = C.Measurements.Table.Helpers.bounds([|measurement|]);
    C.Measurements.Table.Helpers.smallDistribution(
      ~measurement,
      ~bounds,
      ~width=75,
      ~height=30,
      (),
    )
    |> E.O.default("No" |> Utils.ste);
  };

  let competitiveMeasurement =
    Table.Column.make(
      ~name="Prediction" |> Utils.ste,
      ~render=
        (r: record) =>
          r.competitiveMeasurement
          |> E.O.fmap(getMeasurement)
          |> E.O.default("_" |> Utils.ste),
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
          |> E.O.default("_" |> Utils.ste),
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
          |> E.O.default("-" |> Utils.ste),
      ~flex=1,
      (),
    );

  let default = [|measurable, agent, score, time|];
  let measurables = [|
    measurable,
    agent,
    competitiveMeasurement,
    aggregationMeasurement,
    objectiveMeasurement,
    totalScore,
    predictionCount,
    time,
  |];
  let members = [|agent, score, predictedMeasurablesCount, predictionCount|];
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