type user = {
  id: string,
  name: string,
  agentId: string,
};

type bot = {
  competitorType: Types.competitorType,
  description: option(string),
  id: string,
  name: string,
};

type agent = {
  id: string,
  name: option(string),
  measurementCount: int,
  bot: option(bot),
  user: option(user),
};

let toUser = (user: user): Types.user =>
  Primary.User.make(~id=user.id, ~name=user.name, ~agentId=user.agentId, ());

let toBot = (bot: bot): Types.bot => {
  competitorType: bot.competitorType,
  description: bot.description,
  id: bot.id,
  name: Some(bot.name),
  token: None,
  agent: None,
  permissions: None,
};

let toAgent = (agent: agent): Types.agent => {
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
      ) @bsRecord {
        id
        name
        measurementCount
        user: User @bsRecord{
          id
          name
          agentId
        }
        bot: Bot @bsRecord{
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