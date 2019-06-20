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
  "relevantAt": option(MomentRe.Moment.t),
  "competitorType": Context.Primary.CompetitorType.t,
  "createdAt": MomentRe.Moment.t,
  "taggedMeasurementId": option(string),
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
             Context.Primary.Types.Bot(
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
             Context.Primary.Types.User(
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
    ~value=measurement##value |> MeasurementValue.decodeGraphql,
    ~competitorType=measurement##competitorType,
    ~taggedMeasurementId=measurement##taggedMeasurementId,
    ~createdAt=Some(measurement##createdAt),
    ~relevantAt=measurement##relevantAt,
    ~agent,
    (),
  );
};

module Query = [%graphql
  {|
    query getMeasurements(
        $measurableId: String
        $first: Int
        $last: Int
        $after: String
        $before: String
     ) {
        measurements: measurements(
            measurableId: $measurableId
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
                }
                relevantAt @bsDecoder(fn: "E.J.O.toMoment")
                competitorType
                description
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

let componentMaker = (query, innerComponentFn) =>
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

let component =
    (~measurableId, ~pageLimit, ~direction: direction, ~innerComponentFn) => {
  let query =
    queryDirection(
      ~pageLimit,
      ~direction,
      ~fn=Query.make(~measurableId),
      (),
    );
  componentMaker(query, innerComponentFn);
};