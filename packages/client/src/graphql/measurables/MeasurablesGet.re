let toAgent = agent => {
  let agentType =
    agent |> E.O.bind(_, Primary.AgentType.getAgentType(~agent=_, ()));

  agent
  |> E.O.fmap(k =>
       Primary.Agent.make(~id=k##id, ~agentType, ~name=k##name, ())
     );
};

let toSeries = c =>
  Primary.Series.make(
    ~id=c##id,
    ~description=c##description,
    ~name=c##name,
    ~channelId=c##channelId,
    (),
  );

let toMeasurable = m => {
  let channel =
    m##channel
    |> E.O.fmap(channel =>
         Primary.Channel.make(
           ~id=channel##id,
           ~name=channel##name |> E.J.toString,
           ~isArchived=channel##isArchived,
           ~isPublic=channel##isPublic,
           ~myRole=Some(channel##myRole),
           (),
         )
       );

  let outcome =
    m##outcome
    |> E.O.fmap(measurement =>
         Primary.Measurement.make(
           ~id=measurement##id,
           ~valueText=measurement##valueText,
           ~value=measurement##value |> MeasurementValue.decodeGraphql,
           (),
         )
       );

  let previousAggregate =
    m##previousAggregate
    |> E.O.fmap(measurement =>
         Primary.Measurement.make(
           ~id=measurement##id,
           ~valueText=measurement##valueText,
           ~value=measurement##value |> MeasurementValue.decodeGraphql,
           (),
         )
       );

  let recentMeasurement =
    m##recentMeasurement
    |> E.O.fmap(measurement =>
         Primary.Measurement.make(
           ~id=measurement##id,
           ~valueText=measurement##valueText,
           ~value=measurement##value |> MeasurementValue.decodeGraphql,
           (),
         )
       );

  let allowMutations =
    m##permissions##mutations##allow |> E.A.O.concatSome |> E.A.to_list;

  let permissions = Primary.Permissions.make(allowMutations);

  Primary.Measurable.make(
    ~id=m##id,
    ~name=m##name |> E.J.toString,
    ~channelId=m##channelId,
    ~channel,
    ~isArchived=Some(m##isArchived),
    ~valueType=m##valueType,
    ~labelCustom=m##labelCustom,
    ~resolutionEndpoint=m##resolutionEndpoint,
    ~measurementCount=m##measurementCount,
    ~measurerCount=m##measurerCount,
    ~labelSubject=m##labelSubject,
    ~labelOnDate=m##labelOnDate,
    ~labelProperty=m##labelProperty,
    ~createdAt=Some(m##createdAt),
    ~updatedAt=Some(m##updatedAt),
    ~expectedResolutionDate=m##expectedResolutionDate,
    ~state=Some(m##state),
    ~stateUpdatedAt=m##stateUpdatedAt,
    ~creator=m##creator |> toAgent,
    ~series=E.O.fmap(toSeries, m##series),
    ~iAmOwner=Some(m##iAmOwner),
    ~min=m##min,
    ~max=m##max,
    ~outcome,
    ~previousAggregate,
    ~recentMeasurement,
    ~permissions=Some(permissions),
    (),
  );
};

module Query = [%graphql
  {|
    query measurables (
        $measurableIds: [String!]
        $states: [measurableState!]
        $channelId: String
        $seriesId: String
        $creatorId: String
        $first: Int500
        $last: Int500
        $after: Cursor
        $before: Cursor
        $order: [OrderMeasurables]
    ) {
        measurables (
            measurableIds: $measurableIds
            states: $states
            channelId: $channelId
            seriesId: $seriesId
            creatorId: $creatorId
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
              channelId
              isArchived
              channel {
                id
                name
                description
                isPublic
                isArchived
                myRole
              }
              labelCustom
              resolutionEndpoint
              valueType
              measurementCount
              measurerCount
              labelSubject
              labelProperty
              iAmOwner
              min
              max
              state
              labelOnDate @bsDecoder(fn: "E.J.O.toMoment")
              stateUpdatedAt @bsDecoder(fn: "E.J.O.toMoment")
              expectedResolutionDate @bsDecoder(fn: "E.J.O.toMoment")
              createdAt @bsDecoder(fn: "E.J.toMoment")
              updatedAt @bsDecoder(fn: "E.J.toMoment")
              creator {
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
              series {
                id
                name
                description
                channelId
              }
              outcome {
                  id
                  valueText
                  value {
                      floatCdf { xs ys }
                      floatPoint
                      percentage
                      binary
                      unresolvableResolution
                      comment
                  }
              }
              previousAggregate {
                  id
                  valueText
                  value {
                      floatCdf { xs ys }
                      floatPoint
                      percentage
                      binary
                      unresolvableResolution
                      comment
                  }
              }
              recentMeasurement {
                id
                valueText
                value {
                  floatCdf { xs ys }
                  floatPoint
                  percentage
                  binary
                  unresolvableResolution
                  comment
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
  a##measurables |> E.O.fmap(Primary.Connection.fromJson(toMeasurable));

type order =
  option(
    array(
      option({
        .
        "direction": Types.direction,
        "field": Types.fieldMeasurables,
      }),
    ),
  );

let orderMeasurables: order =
  Some([|
    Some({"field": `stateOrder, "direction": `ASC}),
    Some({"field": `refreshedAt, "direction": `DESC}),
  |]);

let queryDirection =
    (
      ~states=?,
      ~seriesId=?,
      ~channelId=?,
      ~creatorId=?,
      ~measurableIds,
      ~pageLimit,
      ~direction,
      ~order=orderMeasurables,
      (),
    ) => {
  let fn =
    Query.make(
      ~seriesId?,
      ~channelId?,
      ~creatorId?,
      ~measurableIds?,
      ~states?,
      ~order?,
    );
  switch ((direction: Primary.Connection.direction)) {
  | None => fn(~first=pageLimit, ())
  | After(after) => fn(~first=pageLimit, ~after, ())
  | Before(before) => fn(~last=pageLimit, ~before, ())
  };
};

let componentMaker = (~pollInterval=?, query, innerComponentFn) =>
  <QueryComponent
    variables=query##variables ?pollInterval fetchPolicy="no-cache">
    ...{o =>
      o.result
      |> HttpResponse.fromApollo
      |> HttpResponse.fmap(unpackEdges)
      |> HttpResponse.optionalToMissing
      |> innerComponentFn
    }
  </QueryComponent>;

let component =
    (
      ~creatorId=None,
      ~seriesId=None,
      ~channelId=None,
      ~measurableIds=None,
      ~states=None,
      ~pollInterval=?,
      ~pageLimit,
      ~direction,
      ~innerComponentFn,
      (),
    ) => {
  let query =
    queryDirection(
      ~channelId?,
      ~seriesId?,
      ~creatorId?,
      ~states?,
      ~measurableIds,
      ~pageLimit,
      ~direction,
      (),
    );
  componentMaker(~pollInterval?, query, innerComponentFn);
};