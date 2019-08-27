type measurable = {
  .
  "id": string,
  "name": string,
  "channelId": string,
  "state": Types.measurableState,
  "valueType": Types.valueType,
  "stateUpdatedAt": option(MomentRe.Moment.t),
  "expectedResolutionDate": option(MomentRe.Moment.t),
};

type measurementScoreSet = {. "primaryPointScore": option(float)};

type measurement = {
  .
  "id": string,
  "agent": option(Types.agentTypeJs),
  "description": option(string),
  "valueText": option(string),
  "relevantAt": option(MomentRe.Moment.t),
  "competitorType": Types.competitorType,
  "createdAt": MomentRe.Moment.t,
  "taggedMeasurementId": option(string),
  "measurable": option(measurable),
  "value": MeasurementValue.graphQlResult,
  "measurementScoreSet": option(measurementScoreSet),
};

type measurements = option({. "edges": option(Js.Array.t(measurement))});

let toMeasurement = (measurement: measurement): Types.measurement => {
  let agentType =
    measurement##agent |> E.O.bind(_, Primary.AgentType.getAgentType);

  let agent =
    measurement##agent
    |> Rationale.Option.fmap(k =>
         Primary.Agent.make(~id=k##id, ~agentType, ~name=k##name, ())
       );

  let measurementScoreSet =
    measurement##measurementScoreSet
    |> Rationale.Option.fmap(k =>
         Primary.MeasurementScoreSet.make(
           ~primaryPointScore=k##primaryPointScore,
           (),
         )
       );

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
    ~measurable=
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
      },
    (),
  );
};

module Query = [%graphql
  {|
    query getMeasurements(
        $measurableId: String
        $agentId: String
        $channelId: String
        $first: Int
        $last: Int
        $after: String
        $before: String
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

                  agent: Agent {
                      id
                      name
                      user: User {
                          id
                          name
                          agentId
                      }
                      bot: Bot {
                          id
                          name
                          competitorType
                      }
                  }

                  measurable: Measurable {
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
                  }
              }
          }
        }
    }
  |}
];

module QueryComponent = ReasonApollo.CreateQuery(Query);

type measurableStates = Types.measurableState;

type inputType('a) =
  (~first: int=?, ~last: int=?, ~after: string=?, ~before: string=?, unit) =>
  'a;

type direction = Primary.Connection.direction;

let queryDirection = (~pageLimit, ~direction, ~fn: inputType('a), ()) =>
  switch ((direction: direction)) {
  | None => fn(~first=pageLimit, ())
  | After(after) => fn(~first=pageLimit, ~after, ())
  | Before(before) => fn(~last=pageLimit, ~before, ())
  };

let unpackResults = result =>
  result##measurements
  |> Rationale.Option.fmap(Primary.Connection.fromJson(toMeasurement));

let componentMaker = (query, innerComponentFn) =>
  QueryComponent.make(~variables=query##variables, response =>
    response.result
    |> HttpResponse.fromApollo
    |> HttpResponse.fmap(unpackResults)
    |> HttpResponse.optionalToMissing
    |> innerComponentFn
  )
  |> ReasonReact.element;

let component =
    (
      ~measurableId=None,
      ~agentId=None,
      ~channelId=None,
      ~measurableState=None,
      ~competitorType=None,
      ~pageLimit,
      ~direction: direction,
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