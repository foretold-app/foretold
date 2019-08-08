open Rationale.Function.Infix;

type common = {
  item: string,
  description: string,
};

type measurable = {
  item: string,
  description: string,
  measurableId: string,
};

type body = {
  common: option(common),
  measurable: option(measurable),
};

type channel = {
  id: string,
  name: string,
  isArchived: bool,
  isPublic: bool,
};

type node = {
  id: string,
  channelId: string,
  body,
  channel,
  createdAt: MomentRe.Moment.t,
  updatedAt: MomentRe.Moment.t,
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

let toMeasurable =
    (m: option(measurable)): option(FeedItemBody.Measurable.t) => {
  switch (m) {
  | Some(measurable) =>
    FeedItemBody.Measurable.make(
      ~item=measurable.item,
      ~description=measurable.description,
      ~measurableId=measurable.measurableId,
      (),
    )
    |> E.O.some
  | _ => None
  };
};

let toBody = (m: body): FeedItemBody.t => {
  FeedItemBody.make(
    ~common=toCommon(m.common),
    ~measurable=toMeasurable(m.measurable),
    (),
  );
};

let toChannel = (m: channel): Types.channel => {
  Primary.Channel.make(
    ~id=m.id,
    ~name=m.name,
    ~isArchived=m.isArchived,
    ~isPublic=m.isPublic,
    (),
  );
};

let toFeedItem = (m: node): Types.feedItem => {
  Primary.FeedItem.make(
    ~id=m.id,
    ~channelId=m.channelId,
    ~body=toBody(m.body),
    ~channel=toChannel(m.channel),
    ~createdAt=Some(m.createdAt),
    ~updatedAt=Some(m.updatedAt),
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
                measurable @bsRecord {
                  item
                  description
                  measurableId
                 }
              }
              channel: Channel @bsRecord {
                id
                name
                isArchived
                isPublic
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