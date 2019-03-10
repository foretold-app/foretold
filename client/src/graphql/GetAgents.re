open Rationale;

type user = {
  id: string,
  name: string,
};

let toUser = (u: user): DataModel.user => {id: u.id, name: u.name};

type bot = {
  competitorType: [ | `AGGREGATION | `COMPETITIVE | `OBJECTIVE],
  description: option(string),
  id: string,
  name: option(string),
};

let toBot = (a: bot): DataModel.bot => {
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

let toAgent = (a: agent): DataModel.agent => {
  let agentType: option(DataModel.agentType) =
    switch (a.bot, a.user) {
    | (Some(bot), None) => Some(Bot(toBot(bot)))
    | (None, Some(user)) => Some(User(toUser(user)))
    | _ => None
    };
  {id: a.id, measurementCount: a.measurementCount, agentType, name: None};
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