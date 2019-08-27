type measurable = {
  .
  "id": string,
  "name": string,
  "channelId": string,
};

type measurements = {. "total": option(int)};

type node = {
  .
  "id": string,
  "agent": {
    .
    "bot":
      option({
        .
        "id": string,
        "name": string,
        "competitorType": Types.competitorType,
      }),
    "id": string,
    "name": option(string),
    "user":
      option({
        .
        "id": string,
        "name": string,
        "agentId": string,
      }),
  },
  "measurable": measurable,
  "createdAt": MomentRe.Moment.t,
  "primaryPointScore": float,
  "measurements": option(measurements),
};

type edges = option({. "edges": option(Js.Array.t(node))});

let toNode = node => {
  let measurable = node##measurable;
  let agent = node##agent;

  // @todo: duplicated
  let agentType =
    switch (agent##bot, agent##user) {
    | (Some(bot), None) =>
      Some(
        Types.Bot(
          Primary.Bot.make(
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
          Primary.User.make(
            ~id=user##id,
            ~name=user##name,
            ~agentId=user##agentId,
            (),
          ),
        ),
      )
    | (_, _) => None
    };

  let agent =
    Primary.Agent.make(~id=agent##id, ~agentType, ~name=agent##name, ());

  let measurable =
    Primary.Measurable.make(
      ~id=measurable##id,
      ~name=measurable##name,
      ~channelId=measurable##channelId,
      (),
    );

  let predictionCountTotal =
    node##measurements
    |> E.O.fmap(measurements =>
         measurements##total |> E.O.fmap(total => total) |> E.O.default(0)
       )
    |> E.O.default(0);

  Primary.AgentMeasurable.make(
    ~id=node##id,
    ~primaryPointScore=node##primaryPointScore,
    ~predictionCountTotal,
    ~agent,
    ~measurable,
    (),
  );
};

module Query = [%graphql
  {|
    query getMeasurements(
        $channelId: String
        $first: Int
        $last: Int
        $after: String
        $before: String
     ) {
        edges: agentMeasurables (
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
              node{
                  id
                  createdAt @bsDecoder(fn: "E.J.toMoment")
                  primaryPointScore
                  agent {
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
                  measurable {
                    id
                    name
                    channelId
                  }
                  measurements {
                    total
                  }
              }
          }
        }
    }
  |}
];

module QueryComponent = ReasonApollo.CreateQuery(Query);

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
  result##edges |> Rationale.Option.fmap(Primary.Connection.fromJson(toNode));

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
      ~channelId=None,
      ~pageLimit,
      ~direction: direction,
      ~innerComponentFn,
      (),
    ) => {
  let query =
    queryDirection(~pageLimit, ~direction, ~fn=Query.make(~channelId?), ());
  componentMaker(query, innerComponentFn);
};