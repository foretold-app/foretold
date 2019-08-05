open Rationale.Function.Infix;

type common = {
  item: string,
  description: string,
};

type body = {common: option(common)};

type node = {
  id: string,
  channelId: string,
  body,
};

let toCommon = (m: option(common)): option(FeedItemBody.Common.t) => {
  switch (m) {
  | Some(common) =>
    FeedItemBody.Common.make(
      ~item=common.item,
      ~description=common.description,
      (),
    )
    |> E.O.some
  | _ => None
  };
};

let toBody = (m: body): FeedItemBody.t => {
  FeedItemBody.make(~common=toCommon(m.common), ());
};

let toFeedItem = (m: node): Types.feedItem => {
  Primary.FeedItem.make(
    ~id=m.id,
    ~channelId=m.channelId,
    ~body=toBody(m.body),
    (),
  );
};

module Query = [%graphql
  {|
    query getFeedItems (
        $channelId: String
        $first: Int
        $last: Int
        $after: String
        $before: String
    ) {
        feedItems: feedItems (
            channelId: $channelId
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
            node @bsRecord{
              id
              channelId
              body @bsRecord {
                common @bsRecord {
                  item
                  description
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
  a##feedItems |> E.O.fmap(Primary.Connection.fromJson(toFeedItem));

let queryToComponent = (query, innerComponentFn) =>
  <QueryComponent variables=query##variables>
    ...{o =>
      o.result
      |> ApolloUtils.apolloResponseToResult
      |> E.R.fmap(unpackEdges ||> innerComponentFn)
      |> E.R.id
    }
  </QueryComponent>;

type inputType('a) =
  (
    ~channelId: string=?,
    ~first: int=?,
    ~last: int=?,
    ~after: string=?,
    ~before: string=?,
    unit
  ) =>
  'a;

type direction = Primary.Connection.direction;

let queryDirection =
    (~channelId=?, ~pageLimit, ~direction, ~fn: inputType('a), ()) => {
  let fn = fn(~channelId?);
  switch ((direction: direction)) {
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
    (~channelId, ~pageLimit, ~direction: direction, ~innerComponentFn) => {
  let query =
    queryDirection(~channelId, ~pageLimit, ~direction, ~fn=Query.make, ());
  componentMaker(query, innerComponentFn);
};