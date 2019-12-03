[@bs.config {jsx: 3}];

type generic = {
  item: string,
  description: string,
};

type measurable = {
  item: string,
  description: string,
  measurableId: string,
};

type body = {
  generic: option(generic),
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

let toCommon = (m: option(generic)): option(FeedItemBody.Generic.t) => {
  switch (m) {
  | Some(generic) =>
    FeedItemBody.Generic.make(
      ~item=generic.item,
      ~description=generic.description,
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
    ~generic=toCommon(m.generic),
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
            node @bsRecord{
              id
              channelId
              body @bsRecord {
                generic @bsRecord {
                  item
                  description
                 }
                measurable @bsRecord {
                  item
                  description
                  measurableId
                 }
              }
              channel @bsRecord {
                id
                name @bsDecoder(fn: "E.J.toString")
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