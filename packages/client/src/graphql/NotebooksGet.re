let toNotebook = m => {
  Primary.Notebook.make(
    ~id=m##id,
    ~ownerId=m##ownerId,
    ~channelId=m##channelId,
    ~createdAt=m##createdAt,
    ~updatedAt=m##updatedAt,
    (),
  );
};

module Query = [%graphql
  {|
    query getNotebooks (
        $channelId: String
        $ownerId: String
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
              ownerId
              channelId
              createdAt
              updatedAt
            }
          }
        }
    }
  |}
];

module QueryComponent = ReasonApollo.CreateQuery(Query);

let unpackEdges = a =>
  a##notebooks |> E.O.fmap(Primary.Connection.fromJson(toNotebook));

let queryDirection = (~channelId=?, ~ownerId=?, ~pageLimit, ~direction, ()) => {
  let fn = Query.make(~channelId?, ~ownerId?);
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
