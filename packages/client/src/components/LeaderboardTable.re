[@bs.config {jsx: 3}];

module Columns = {
  type record = Types.measurement;
  type column = Table.column(Types.measurement);

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

  let measurable: column =
    Table.Column.make(
      ~name="Question" |> Utils.ste,
      ~render=(r: record) => r |> getName,
      ~flex=2,
      (),
    );

  let agent: column =
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

  let score: column =
    Table.Column.make(
      ~name="Score" |> Utils.ste,
      ~render=
        (r: record) =>
          switch (r.measurementScoreSet) {
          | Some(measurementScoreSet) =>
            measurementScoreSet.primaryPointScore
            |> E.O.fmap(primaryPointScore =>
                 primaryPointScore |> string_of_float |> Utils.ste
               )
            |> E.O.default("0.0" |> Utils.ste)
          | _ => "0.0" |> Utils.ste
          },
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

  let all = [|measurable, agent, score, time|];
};

[@react.component]
let make = (~items, ~columns=Columns.all) =>
  Table.fromColumns(columns, items, ());

module Jsx2 = {
  let component = ReasonReact.statelessComponent("LeaderboardTable");
  let make = (~items, ~columns=Columns.all, children) =>
    ReasonReactCompat.wrapReactForReasonReact(
      make,
      makeProps(~items, ~columns, ()),
      children,
    );
};