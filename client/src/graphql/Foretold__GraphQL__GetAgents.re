type user = {
  id: string,
  name: string,
};

let toUser = (u: user): DataModel.User.t =>
  DataModel.User.make(~id=u.id, ~name=u.name, ());

type bot = {
  competitorType: [ | `AGGREGATION | `COMPETITIVE | `OBJECTIVE],
  description: option(string),
  id: string,
  name: option(string),
};

let toBot = (a: bot): DataModel.Bot.t => {
  competitorType: a.competitorType,
  description: a.description,
  id: a.id,
  name: a.name,
};

type agent = {
  id: string,
  measurementCount: option(int),
  bot: option(bot),
  user: option(user),
};

let toAgent = (a: agent): DataModel.Agent.t => {
  let agentType: option(DataModel.AgentType.t) =
    switch (a.bot, a.user) {
    | (Some(bot), None) => Some(Bot(toBot(bot)))
    | (None, Some(user)) => Some(User(toUser(user)))
    | _ => None
    };
  DataModel.Agent.make(
    ~id=a.id,
    ~measurementCount=a.measurementCount,
    ~agentType,
    ~name=None,
    (),
  );
};

type agents = array(agent);

module Query = [%graphql
  {|
    query getAgents {
      agents @bsRecord{
        id
        measurementCount
        user: User @bsRecord{
          id
          name
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