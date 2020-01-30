module Query = [%graphql
  {|
    query notebooks (
        $channelId: ChannelId
        $ownerId: AgentId
        $first: Int500
        $last: Int500
        $after: Cursor
        $before: Cursor
    ) {
        notebooks (
            channelId: $channelId
            ownerId: $ownerId
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
              name
              body
              ownerId
              channelId
              createdAt
              updatedAt
              channel {
                id
                name @bsDecoder(fn: "E.J.toString")
                isArchived
                isPublic
              }
              owner {
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
            }
          }
        }
    }
  |}
];

module QueryComponent = ReasonApollo.CreateQuery(Query);

let unpackEdges = a =>
  a##notebooks
  |> E.O.fmap(Primary.Connection.fromJson(Primary.Notebook.convertJsObject));

let queryDirection = (~channelId=?, ~ownerId=?, ~pageLimit, ~direction, ()) => {
  let fn = Query.make(~channelId?, ~ownerId?);
  switch ((direction: Primary.Connection.direction)) {
  | None => fn(~first=pageLimit, ())
  | After(after) => fn(~first=pageLimit, ~after, ())
  | Before(before) => fn(~last=pageLimit, ~before, ())
  };
};

// @todo: Fix "no-cache"
let componentMaker = (query, innerComponentFn) =>
  <QueryComponent variables=query##variables fetchPolicy="no-cache">
    ...{o =>
      o.result
      |> HttpResponse.fromApollo
      |> HttpResponse.fmap(unpackEdges)
      |> HttpResponse.optionalToMissing
      |> innerComponentFn
    }
  </QueryComponent>;

let component =
    (~channelId: string, ~pageLimit, ~direction, ~innerComponentFn) => {
  let channelId = channelId |> E.J.fromString;
  let query = queryDirection(~channelId, ~pageLimit, ~direction, ());
  componentMaker(query, innerComponentFn);
};