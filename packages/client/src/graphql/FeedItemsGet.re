type channel = {
  id: string,
  name: string,
  isArchived: bool,
  isPublic: bool,
};

type node = {
  id: string,
  channelId: string,
  body: FeedItemBody.t,
  channel,
  createdAt: MomentRe.Moment.t,
  updatedAt: MomentRe.Moment.t,
};

let toCommon =
    (m: option(FeedItemBody.generic)): option(FeedItemBody.Generic.t) => {
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
    (m: option(FeedItemBody.measurable)): option(FeedItemBody.Measurable.t) => {
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

let toMeasurement =
    (m: option(FeedItemBody.measurement))
    : option(FeedItemBody.Measurement.t) => {
  switch (m) {
  | Some(measurement) =>
    FeedItemBody.Measurement.make(
      ~item=measurement.item,
      ~description=measurement.description,
      ~measurableId=measurement.measurableId,
      ~measurementId=measurement.measurementId,
      (),
    )
    |> E.O.some
  | _ => None
  };
};

let toNotebook =
    (m: option(FeedItemBody.notebook)): option(FeedItemBody.Notebook.t) => {
  switch (m) {
  | Some(notebook) =>
    FeedItemBody.Notebook.make(
      ~item=notebook.item,
      ~description=notebook.description,
      ~notebookId=notebook.notebookId,
      (),
    )
    |> E.O.some
  | _ => None
  };
};

let toJoinedMember =
    (m: option(FeedItemBody.joinedMember))
    : option(FeedItemBody.JoinedMember.t) => {
  switch (m) {
  | Some(joinedMember) =>
    FeedItemBody.JoinedMember.make(
      ~item=joinedMember.item,
      ~description=joinedMember.description,
      (),
    )
    |> E.O.some
  | _ => None
  };
};

let toChannel2 =
    (m: option(FeedItemBody.channel)): option(FeedItemBody.Channel.t) => {
  switch (m) {
  | Some(channel2) =>
    FeedItemBody.Channel.make(
      ~item=channel2.item,
      ~description=channel2.description,
      (),
    )
    |> E.O.some
  | _ => None
  };
};

let toBody = (m: FeedItemBody.t) => {
  FeedItemBody.make(
    ~generic=toCommon(m.generic),
    ~measurable=toMeasurable(m.measurable),
    ~measurement=toMeasurement(m.measurement),
    ~notebook=toNotebook(m.notebook),
    ~channel=toChannel2(m.channel),
    ~joinedMember=toJoinedMember(m.joinedMember),
    (),
  );
};

let toChannel = (m: channel) => {
  Primary.Channel.make(
    ~id=m.id,
    ~name=m.name,
    ~isArchived=m.isArchived,
    ~isPublic=m.isPublic,
    (),
  );
};

let toFeedItem = (m: node) => {
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
                 measurement @bsRecord {
                   item
                   description
                   measurableId
                   measurementId
                 }
                 joinedMember @bsRecord {
                   item
                   description
                 }
                 notebook @bsRecord {
                   item
                   description
                 }
                 channel @bsRecord {
                   item
                   description
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