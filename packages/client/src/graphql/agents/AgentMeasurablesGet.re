[@bs.config {jsx: 3}];

let toNode = node => {
  let measurable = node##measurable;
  let agent = node##agent;

  let agentType = Primary.AgentType.getAgentType(~agent, ());
  let agent =
    Primary.Agent.make(~id=agent##id, ~agentType, ~name=agent##name, ());

  let measurable =
    Primary.Measurable.make(
      ~id=measurable##id,
      ~name=measurable##name,
      ~channelId=measurable##channelId,
      (),
    );

  Primary.AgentMeasurable.make(
    ~id=node##id,
    ~primaryPointScore=node##primaryPointScore,
    ~primaryPointScoreConstant=node##primaryPointScoreConstant,
    ~createdAt=node##createdAt,
    ~predictionCountTotal=node##predictionCountTotal,
    ~agent,
    ~measurable,
    (),
  );
};

module Query = [%graphql
  {|
    query getAgentMeasurables(
        $first: Int500
        $last: Int500
        $after: Cursor
        $before: Cursor
        $channelId: String
        $measurableId: String
        $minPredictionCountTotal: Int
        $measurableState: [measurableState]
        $order: [Order]
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
                  primaryPointScore
                  primaryPointScoreConstant
                  predictionCountTotal
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
  | None => fn(~first=pageLimit, ())
  | After(after) => fn(~first=pageLimit, ~after, ())
  | Before(before) => fn(~last=pageLimit, ~before, ())
  };

let unpackResults = result =>
  result##edges |> Rationale.Option.fmap(Primary.Connection.fromJson(toNode));

let componentMaker = (query, innerComponentFn) =>
  <QueryComponent variables=query##variables>
    ...{o =>
      o.result
      |> HttpResponse.fromApollo
      |> HttpResponse.fmap(unpackResults)
      |> HttpResponse.optionalToMissing
      |> innerComponentFn
    }
  </QueryComponent>;

let component =
    (
      ~channelId=None,
      ~measurableId=None,
      ~measurableState=Some([|Some(`JUDGED)|]),
      ~minPredictionCountTotal=Some(1),
      ~order=Some([|
               Some({"field": `primaryPointScore, "direction": `DESC}),
             |]),
      ~pageLimit,
      ~direction,
      ~innerComponentFn,
      (),
    ) => {
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
        ),
      (),
    );
  componentMaker(query, innerComponentFn);
};
