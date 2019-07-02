type measurable = {
  .
  "id": string,
  "name": string,
  "channelId": string,
  "state": Context.Primary.MeasurableState.t,
  "valueType": Types.valueType,
  "stateUpdatedAt": option(MomentRe.Moment.t),
  "expectedResolutionDate": option(MomentRe.Moment.t),
};

type measurement = {
  .
  "id": string,
  "agent":
    option({
      .
      "bot":
        option({
          .
          "id": string,
          "name": string,
          "competitorType": Context.Primary.CompetitorType.t,
        }),
      "id": string,
      "name": option(string),
      "user":
        option({
          .
          "id": string,
          "name": string,
        }),
    }),
  "description": option(string),
  "valueText": option(string),
  "relevantAt": option(MomentRe.Moment.t),
  "competitorType": Context.Primary.CompetitorType.t,
  "createdAt": MomentRe.Moment.t,
  "taggedMeasurementId": option(string),
  "measurable": option(measurable),
  "value": MeasurementValue.graphQlResult,
};

type measurements = option({. "edges": option(Js.Array.t(measurement))});

let toMeasurement = (measurement: measurement): Context.Primary.Measurement.t => {
  open Context.Primary.Agent;
  let agent = measurement##agent;

  let agentType: option(Context.Primary.AgentType.t) =
    agent
    |> E.O.bind(_, k =>
         switch (k##bot, k##user) {
         | (Some(bot), None) =>
           Some(
             Types.Bot(
               Context.Primary.Bot.make(
                 ~id=bot##id,
                 ~name=Some(bot##name),
                 ~competitorType=bot##competitorType,
                 (),
               ),
             ),
           )
         | (None, Some(user)) =>
           Some(
             Types.User(
               Context.Primary.User.make(~id=user##id, ~name=user##name, ()),
             ),
           )
         | (_, _) => None
         }
       );

  let agent: option(Context.Primary.Agent.t) =
    agent
    |> Rationale.Option.fmap(k =>
         Context.Primary.Agent.make(~id=k##id, ~agentType, ())
       );

  Context.Primary.Measurement.make(
    ~id=measurement##id,
    ~description=measurement##description,
    ~valueText=measurement##valueText,
    ~value=measurement##value |> MeasurementValue.decodeGraphql,
    ~competitorType=measurement##competitorType,
    ~taggedMeasurementId=measurement##taggedMeasurementId,
    ~createdAt=Some(measurement##createdAt),
    ~relevantAt=measurement##relevantAt,
    ~agent,
    ~measurable=
      switch (measurement##measurable) {
      | Some(measurable) =>
        Some(
          Context.Primary.Measurable.make(
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
        $first: Int
        $last: Int
        $after: String
        $before: String
     ) {
        measurements: measurements(
            measurableId: $measurableId
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
              }
          }
        }
    }
  |}
];

module QueryComponent = ReasonApollo.CreateQuery(Query);

type measurementEdges = Client.Context.Primary.Connection.edges(measurement);

let queryToComponent = (query, innerComponentFn) =>
  QueryComponent.make(~variables=query##variables, response =>
    response.result
    |> ApolloUtils.apolloResponseToResult
    |> Rationale.Result.fmap(result =>
         result##measurements
         |> Rationale.Option.fmap(
              Context.Primary.Connection.fromJson(toMeasurement),
            )
         |> innerComponentFn
       )
    |> E.R.id
  )
  |> ReasonReact.element;

type measurableStates = Context.Primary.MeasurableState.t;

type inputType('a) =
  (~first: int=?, ~last: int=?, ~after: string=?, ~before: string=?, unit) =>
  'a;

type direction = Context.Primary.Connection.direction;

let queryDirection = (~pageLimit, ~direction, ~fn: inputType('a), ()) =>
  switch ((direction: direction)) {
  | None => fn(~first=pageLimit, ())
  | After(after) => fn(~first=pageLimit, ~after, ())
  | Before(before) => fn(~last=pageLimit, ~before, ())
  };

let unpackResults = result =>
  result##measurements
  |> Rationale.Option.fmap(
       Context.Primary.Connection.fromJson(toMeasurement),
     );

let componentMaker = (query, innerComponentFn) =>
  QueryComponent.make(~variables=query##variables, response =>
    response.result
    |> ApolloUtils.apolloResponseToResult
    |> Rationale.Result.fmap(result =>
         result |> unpackResults |> innerComponentFn
       )
    |> E.R.id
  )
  |> ReasonReact.element;

let componentMakerMissingOptional = (query, innerComponentFn) =>
  QueryComponent.make(~variables=query##variables, response =>
    response.result
    |> E.HttpResponse.fromApollo
    |> E.HttpResponse.fmap(unpackResults)
    |> E.HttpResponse.optionalToMissing
    |> innerComponentFn
  )
  |> ReasonReact.element;

let component =
    (~measurableId, ~pageLimit, ~direction: direction, ~innerComponentFn) => {
  let query =
    queryDirection(
      ~pageLimit,
      ~direction,
      ~fn=Query.make(~measurableId, ~agentId=""),
      (),
    );
  componentMaker(query, innerComponentFn);
};

let componentWithAgent =
    (~agentId, ~pageLimit, ~direction: direction, ~innerComponentFn) => {
  let query =
    queryDirection(
      ~pageLimit,
      ~direction,
      ~fn=Query.make(~measurableId="", ~agentId),
      (),
    );
  componentMakerMissingOptional(query, innerComponentFn);
};