let toNode = node => {
  let channel = node##channel;
  let agent = node##agent;

  let agentType = Primary.AgentType.getAgentType(~agent, ());
  let agent =
    Primary.Agent.make(~id=agent##id, ~agentType, ~name=agent##name, ());

  let channel =
    Primary.Channel.make(
      ~id=channel##id,
      ~name=channel##name |> E.J.toString,
      ~isArchived=channel##isArchived,
      ~isPublic=channel##isPublic,
      (),
    );

  Primary.AgentChannel.make(
    ~id=node##id,
    ~agentId=node##agentId,
    ~channelId=node##channelId,
    ~primaryPointScore=node##primaryPointScore,
    ~numberOfPredictions=node##numberOfPredictions,
    ~numberOfQuestionsScored=node##numberOfQuestionsScored,
    ~createdAt=node##createdAt,
    ~updatedAt=node##updatedAt,
    ~totalVotesReceived=node##totalVotesReceived,
    ~agent,
    ~channel,
    (),
  );
};

module Query = [%graphql
  {|
    query agentChannels (
        $first: Int500
        $last: Int500
        $after: Cursor
        $before: Cursor
        $channelId: String
        $agentId: String
        $minNumberOfPredictions: Int
        $minNumberOfQuestionsScored: Int
     ) {
        edges: agentChannels (
            first: $first
            last: $last
            after: $after
            before: $before
            channelId: $channelId
            agentId: $agentId
            minNumberOfPredictions: $minNumberOfPredictions
            minNumberOfQuestionsScored: $minNumberOfQuestionsScored
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
                agentId
                channelId
                primaryPointScore
                numberOfPredictions
                numberOfQuestionsScored
                totalVotesReceived
                createdAt @bsDecoder(fn: "E.J.toMoment")
                updatedAt @bsDecoder(fn: "E.J.toMoment")
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
                channel {
                    id
                    name
                    isArchived
                    isPublic
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
      ~agentId=None,
      ~minNumberOfPredictions=None,
      ~minNumberOfQuestionsScored=None,
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
          ~agentId?,
          ~minNumberOfPredictions?,
          ~minNumberOfQuestionsScored?,
        ),
      (),
    );
  componentMaker(query, innerComponentFn);
};