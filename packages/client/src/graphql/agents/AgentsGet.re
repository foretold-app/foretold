let toUser = (user): Types.user =>
  Primary.User.make(
    ~id=user##id,
    ~name=user##name,
    ~agentId=user##agentId,
    (),
  );

let toBot = (bot: bot): Types.bot =>
  Primary.Bot.make(
    ~id=bot##id,
    ~name=bot##name,
    ~agentId=bot##agentId,
    ~competitorType=bot##competitorType,
    ~description=bot##description,
    (),
  );

let toAgent = (agent): Types.agent => {
  let agentType: option(Primary.AgentType.t) =
    switch (agent.bot, agent.user) {
    | (Some(bot), None) => Some(Bot(toBot(bot)))
    | (None, Some(user)) => Some(User(toUser(user)))
    | _ => None
    };

  Primary.Agent.make(
    ~id=agent.id,
    ~name=agent.name,
    ~measurementCount=Some(agent.measurementCount),
    ~agentType,
    (),
  );
};

type agents = array(agent);

module Query = [%graphql
  {|
    query getAgents (
        $excludeChannelId: String
        $types: [AgentType]
    ) {
      agents (
        excludeChannelId: $excludeChannelId
        types: $types
      ) {
        id
        name
        measurementCount
        user {
          id
          name
          agentId
        }
        bot {
          id
          name
          description
          competitorType
        }
      }
    }
  |}
];

module QueryComponent = ReasonApollo.CreateQuery(Query);

let component = (~excludeChannelId: string, ~types=?, innerFn) => {
  let query = Query.make(~excludeChannelId, ~types?, ());
  <QueryComponent variables=query##variables>
    ...{response =>
      response.result
      |> HttpResponse.fromApollo
      |> HttpResponse.fmap((e: Query.t) => {
           let agents = e##agents;
           let foo = agents |> E.A.O.concatSomes |> E.A.fmap(toAgent);
           foo;
         })
      |> innerFn
    }
  </QueryComponent>;
};

let componentUsers = component(~types=[|Some(`USER)|]);
