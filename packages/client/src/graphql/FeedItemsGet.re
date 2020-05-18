let toChannel = m => {
  Primary.Channel.make(
    ~id=m##id,
    ~name=m##name,
    ~isArchived=m##isArchived,
    ~isPublic=m##isPublic,
    (),
  );
};

let toAgent = agent => {
  let agentType = Primary.AgentType.getAgentType(~agent, ());

  Primary.Agent.make(~id=agent##id, ~agentType, ~name=agent##name, ());
};

let toFeedItem = m => {
  let agent = m##agent |> toAgent;

  Primary.FeedItem.make(
    ~id=m##id,
    ~channelId=m##channelId,
    ~body=FeedItemBody.toBody(m##body),
    ~channel=toChannel(m##channel),
    ~createdAt=Some(m##createdAt),
    ~updatedAt=Some(m##updatedAt),
    ~agent,
    (),
  );
};

module Query = [%graphql
  {|
    query feedItems (
        $channelId: String
        $agentId: String
        $first: Int500
        $last: Int500
        $after: Cursor
        $before: Cursor
    ) {
        feedItems (
            channelId: $channelId
            agentId: $agentId
            first: $first
            last: $last
            after: $after
            before: $before
        ) {
          total
          pageInfo{
            hasPreviousPage
            hasNextPage
            startCursor
            endCursor
          }
          edges{
            node {
              id
              channelId
              body {
                 generic {
                   item
                   description
                 }
                 measurable {
                   item
                   description
                   measurableId
                 }
                 measurableB {
                   item
                   description
                   measurableId
                   labelSubject
                   labelProperty
                   labelCustom
                   labelStartAtDate @bsDecoder(fn: "E.J.O.toMoment")
                   labelEndAtDate @bsDecoder(fn: "E.J.O.toMoment")
                   labelConditionals @bsDecoder(fn: "E.JsArray.toStrings")
                 }
                 measurement {
                   item
                   description
                   measurableId
                   measurementId
                 }
                 joinedMember {
                   item
                   description
                 }
                 notebook {
                   item
                   description
                   notebookId
                 }
                 channel {
                   item
                   description
                 }
                 series {
                   item
                   description
                   seriesId
                 }
              }
              channel {
                id
                name @bsDecoder(fn: "E.J.toString")
                isArchived
                isPublic
              }
              agent {
                id
                name
                user {
                  id
                  name
                  description
                  agentId
                  picture
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
              createdAt @bsDecoder(fn: "E.J.toMoment")
              updatedAt @bsDecoder(fn: "E.J.toMoment")
            }
          }
        }
    }
  |}
];

module QueryComponent = ReasonApollo.CreateQuery(Query);

let unpackEdges = a =>
  a##feedItems |> E.O.fmap(Primary.Connection.fromJson(toFeedItem));

let queryDirection = (~channelId=?, ~agentId=?, ~pageLimit, ~direction, ()) => {
  let fn = Query.make(~channelId?, ~agentId?);
  switch ((direction: Primary.Connection.direction)) {
  | None => fn(~first=pageLimit, ())
  | After(after) => fn(~first=pageLimit, ~after, ())
  | Before(before) => fn(~last=pageLimit, ~before, ())
  };
};

let componentMaker = (query, innerComponentFn) =>
  <QueryComponent variables=query##variables>
    ...{o =>
      o.result
      |> HttpResponse.fromApollo
      |> HttpResponse.fmap(unpackEdges)
      |> HttpResponse.optionalToMissing
      |> innerComponentFn
    }
  </QueryComponent>;

let component2 =
    (~channelId, ~agentId, ~pageLimit, ~direction, ~innerComponentFn) => {
  let query =
    queryDirection(~channelId?, ~agentId?, ~pageLimit, ~direction, ());
  componentMaker(query, innerComponentFn);
};