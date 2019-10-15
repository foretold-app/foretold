let toMeasurement = (measurement): Types.measurement => {
  let agentType =
    measurement##agent
    |> E.O.bind(_, Primary.AgentType.getAgentType(~agent=_, ()));

  let agent =
    measurement##agent
    |> E.O.fmap(k =>
         Primary.Agent.make(~id=k##id, ~agentType, ~name=k##name, ())
       );

  let measurementScoreSet =
    measurement##measurementScoreSet
    |> E.O.fmap(measurementScoreSet =>
         Primary.MeasurementScoreSet.make(
           ~primaryPointScore=measurementScoreSet##primaryPointScore,
           ~nonMarketLogScore=measurementScoreSet##nonMarketLogScore,
           (),
         )
       );

  let measurable =
    switch (measurement##measurable) {
    | Some(measurable) =>
      Some(
        Primary.Measurable.make(
          ~id=measurable##id,
          ~name=measurable##name,
          ~channelId=measurable##channelId,
          ~valueType=measurable##valueType,
          (),
        ),
      )
    | None => None
    };

  Primary.Measurement.make(
    ~id=measurement##id,
    ~description=measurement##description,
    ~valueText=measurement##valueText,
    ~value=measurement##value |> MeasurementValue.decodeGraphql,
    ~competitorType=measurement##competitorType,
    ~taggedMeasurementId=measurement##taggedMeasurementId,
    ~createdAt=Some(measurement##createdAt),
    ~relevantAt=measurement##relevantAt,
    ~agent,
    ~measurementScoreSet,
    ~measurable,
    (),
  );
};

module Query = [%graphql
  {|
    query getMeasurements(
        $measurableId: String
        $agentId: String
        $channelId: String
        $first: Int500
        $last: Int500
        $after: Cursor
        $before: Cursor
        $measurableState: [measurableState]
        $competitorType: [competitorType!]
     ) {
        measurements: measurements(
            measurableId: $measurableId
            agentId: $agentId
            channelId: $channelId
            first: $first
            last: $last
            after: $after
            before: $before
            measurableState: $measurableState
            competitorType: $competitorType
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
                  createdAt @bsDecoder(fn: "E.J.toMoment")
                  value {
                      floatCdf {
                          xs
                          ys
                      }
                      floatPoint
                      percentage
                      binary
                      unresolvableResolution
                      comment
                  }
                  relevantAt @bsDecoder(fn: "E.J.O.toMoment")
                  competitorType
                  description
                  valueText
                  taggedMeasurementId

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

                  measurable {
                    id
                    name
                    channelId
                    expectedResolutionDate @bsDecoder(fn: "E.J.O.toMoment")
                    state
                    valueType
                    stateUpdatedAt @bsDecoder(fn: "E.J.O.toMoment")
                  }

                  measurementScoreSet {
                    primaryPointScore
                    nonMarketLogScore
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
  | None => fn(~first=pageLimit, ())
  | After(after) => fn(~first=pageLimit, ~after, ())
  | Before(before) => fn(~last=pageLimit, ~before, ())
  };

let unpackResults = result =>
  result##measurements
  |> Rationale.Option.fmap(Primary.Connection.fromJson(toMeasurement));

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
      ~measurableId=None,
      ~agentId=None,
      ~channelId=None,
      ~measurableState=None,
      ~competitorType=None,
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
          ~measurableId?,
          ~agentId?,
          ~measurableState?,
          ~competitorType?,
          ~channelId?,
        ),
      (),
    );
  componentMaker(query, innerComponentFn);
};
