module Query = [%graphql
  {|
    query notebooks (
        $channelId: ChannelId
        $ownerId: AgentId
        $first: Int500
        $last: Int500
        $after: Cursor
        $before: Cursor
        $order: [OrderNotebooks]
    ) {
        notebooks (
            channelId: $channelId
            ownerId: $ownerId
            first: $first
            last: $last
            after: $after
            before: $before
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
            node {
              id
              name
              body
              ownerId
              channelId
              createdAt
              updatedAt
              isBookmarked
              bookmarksCount
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
              permissions {
                mutations {
                  allow
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

type order =
  option(
    array(
      option({
        .
        "direction": Types.direction,
        "field": Types.fieldNotebooks,
      }),
    ),
  );

let orderNotebooks: order =
  Some([|Some({"field": `isBookmarked, "direction": `DESC})|]);

let queryDirection =
    (
      ~channelId=?,
      ~ownerId=?,
      ~order=orderNotebooks,
      ~pageLimit,
      ~direction,
      (),
    ) => {
  let fn = Query.make(~channelId?, ~ownerId?, ~order?);
  switch ((direction: Primary.Connection.direction)) {
  | NoneDirection => fn(~first=pageLimit, ())
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