type user = {
  id: string,
  name: string,
};

let toUser = (u: user): Context.Primary.User.t =>
  Context.Primary.User.make(~id=u.id, ~name=u.name, ());

type bot = {
  competitorType: [ | `AGGREGATION | `COMPETITIVE | `OBJECTIVE],
  description: option(string),
  id: string,
  name: option(string),
};

let toBot = (a: bot): Context.Primary.Bot.t => {
  competitorType: a.competitorType,
  description: a.description,
  id: a.id,
  name: a.name,
};

type agent = {
  id: string,
  name: option(string),
  measurementCount: int,
  bot: option(bot),
  user: option(user),
};

let toAgent = (a: agent): Context.Primary.Agent.t => {
  let agentType: option(Context.Primary.AgentType.t) =
    switch (a.bot, a.user) {
    | (Some(bot), None) => Some(Bot(toBot(bot)))
    | (None, Some(user)) => Some(User(toUser(user)))
    | _ => None
    };
  Context.Primary.Agent.make(
    ~id=a.id,
    ~name=a.name,
    ~measurementCount=Some(a.measurementCount),
    ~agentType,
    (),
  );
};

type agents = array(agent);

module Query = [%graphql
  {|
    query getAgents {
      agents @bsRecord{
        id
        name
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

let component = innerFn => {
  let query = Query.make();
  QueryComponent.make(~variables=query##variables, ({result}) =>
    result
    |> E.HttpResponse.fromApollo
    |> E.HttpResponse.fmap((e: Query.t) => {
         let agents = e##agents;
         let foo = agents |> E.A.O.concatSomes |> E.A.fmap(toAgent);
         foo;
       })
    |> innerFn
  )
  |> E.React.el;
};