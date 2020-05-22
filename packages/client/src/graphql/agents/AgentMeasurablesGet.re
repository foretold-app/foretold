let toNode = (marketType, finalComparisonMeasurement, node) => {
  let measurable = node##measurable;
  let agent = node##agent;

  let agentType = Primary.AgentType.getAgentType(~agent, ());
  let agent =
    Primary.Agent.make(~id=agent##id, ~agentType, ~name=agent##name, ());

  let measurable =
    Primary.Measurable.make(
      ~id=measurable##id,
      ~name=measurable##name |> E.J.toString,
      ~channelId=measurable##channelId,
      (),
    );

  let timeAverageScore =
    E.O.bind(node##timeAverageScore, timeAverageScore =>
      switch (timeAverageScore##score, timeAverageScore##timeActivityRatio) {
      | (Some(score), Some(timeActivityRatio)) =>
        Some(
          Primary.TimeAverageScore.make(
            ~score,
            ~timeActivityRatio,
            ~marketType,
            ~finalComparisonMeasurement,
          ),
        )
      | _ => None
      }
    );

  Primary.AgentMeasurable.make(
    ~id=node##id,
    ~createdAt=node##createdAt,
    ~predictionCountTotal=node##predictionCountTotal,
    ~totalVotesReceived=node##totalVotesReceived,
    ~timeAverageScore,
    ~agent,
    ~measurable,
    (),
  );
};

module Query = [%graphql
  {|
    query agentMeasurables (
        $first: Int500
        $last: Int500
        $after: Cursor
        $before: Cursor
        $channelId: String
        $measurableId: String
        $minPredictionCountTotal: Int
        $measurableState: [measurableState]
        $order: [OrderAgentMeasurables]
        $marketType: marketScoreType!
        $finalComparisonMeasurement: finalComparisonMeasurement!
     ) {
        edges: agentMeasurables (
            first: $first
            last: $last
            after: $after
            before: $before
            channelId: $channelId
            measurableId: $measurableId
            minPredictionCountTotal: $minPredictionCountTotal
            measurableState: $measurableState
            order: $order
        ) {
          total
          pageInfo{
            hasPreviousPage
            hasNextPage
            startCursor
            endCursor
          }
          edges{
              node{
                  id
                  createdAt @bsDecoder(fn: "E.J.toMoment")
                  timeAverageScore (
                    marketType: $marketType
                    startAt: AGENT_MEASUREMENT_CREATION_TIME
                    finalComparisonMeasurement: $finalComparisonMeasurement
                  ) {
                    score
                    timeActivityRatio
                  }
                  predictionCountTotal
                  totalVotesReceived
                  agent {
                      id
                      name
                      user {
                          id
                          name
                          description
                          picture
                          agentId
                      }
                      bot {
                          id
                          name
                          description
                          picture
                          competitorType
                          user {
                            id
                            name
                            description
                            picture
                            agentId
                          }
                      }
                  }
                  measurable {
                    id
                    name
                    channelId
                  }
              }
          }
        }
    }
  |}
];

module QueryComponent = ReasonApollo.CreateQuery(Query);

let queryDirection =
    (~pageLimit, ~direction, ~fn: Types.connectionInputType('a), ()) =>
  switch ((direction: Primary.Connection.direction)) {
  | NoneDirection => fn(~first=pageLimit, ())
  | After(after) => fn(~first=pageLimit, ~after, ())
  | Before(before) => fn(~last=pageLimit, ~before, ())
  };

let unpackResults = (marketType, finalComparisonMeasurement, result) =>
  result##edges
  |> Rationale.Option.fmap(
       Primary.Connection.fromJson(
         toNode(marketType, finalComparisonMeasurement),
       ),
     );

let componentMaker =
    (query, marketType, finalComparisonMeasurement, innerComponentFn) =>
  <QueryComponent variables=query##variables>
    ...{o =>
      o.result
      |> HttpResponse.fromApollo
      |> HttpResponse.fmap(
           unpackResults(marketType, finalComparisonMeasurement),
         )
      |> HttpResponse.optionalToMissing
      |> innerComponentFn
    }
  </QueryComponent>;

let component =
    (
      ~channelId=None,
      ~measurableId=None,
      ~measurableState=None,
      ~minPredictionCountTotal=Some(1),
      ~pageLimit,
      ~direction,
      ~innerComponentFn,
      ~order=Some([|
               Some({"field": `primaryPointScore, "direction": `DESC}),
             |]),
      ~marketType=None,
      ~finalComparisonMeasurement=None,
      (),
    ) => {
  let marketType = marketType |> E.O.default(`MARKET);
  let finalComparisonMeasurement =
    finalComparisonMeasurement |> E.O.default(`LAST_OBJECTIVE_MEASUREMENT);
  let query =
    queryDirection(
      ~pageLimit,
      ~direction,
      ~fn=
        Query.make(
          ~channelId?,
          ~measurableId?,
          ~measurableState?,
          ~minPredictionCountTotal?,
          ~order?,
          ~marketType,
          ~finalComparisonMeasurement,
        ),
      (),
    );
  componentMaker(
    query,
    marketType,
    finalComparisonMeasurement,
    innerComponentFn,
  );
};