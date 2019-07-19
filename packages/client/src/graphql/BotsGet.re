module Query = [%graphql
  {|
    query bots ($ownerId: String) {
      bots(first: 200, ownerId: $ownerId){
          edges{
              node{
                  id
                  name
                  description
                  competitorType
                  token
                  agent: Agent{
                    id
                  }
                  permissions {
                    mutations {
                      allow
                    }
                  }
              }
          }
      }
    }
  |}
];

module QueryComponent = ReasonApollo.CreateQuery(Query);

let unpackEdges = (a): array('a) => {
  let response =
    a
    |> E.O.fmap(b => b##edges |> E.A.O.defaultEmpty |> E.A.O.concatSome)
    |> E.A.O.defaultEmpty
    |> E.A.fmap(e => e##node)
    |> E.A.O.concatSome;
  response;
};

type bot = {
  .
  "competitorType": Primary.CompetitorType.t,
  "id": string,
  "description": option(string),
  "token": option(string),
  "agent": option({. "id": string}),
  "name": string,
  "permissions": {
    .
    "mutations": {. "allow": Js.Array.t(option(Types.permission))},
  },
};

let toBot = (botJson: bot) => {
  let allowMutations =
    botJson##permissions##mutations##allow |> E.A.O.concatSome |> E.A.to_list;

  let permissions = Primary.Permissions.make(allowMutations);

  Primary.Bot.make(
    ~id=botJson##id,
    ~name=Some(botJson##name),
    ~description=botJson##description,
    ~competitorType=botJson##competitorType,
    ~token=botJson##token,
    ~agent=
      botJson##agent
      |> E.O.fmap(agentJson => Primary.Agent.make(~id=agentJson##id, ())),
    ~permissions=Some(permissions),
    (),
  );
};

let component = (~ownerId, fn) => {
  let query = Query.make(~ownerId, ());
  QueryComponent.make(~variables=query##variables, ({result}) =>
    result
    |> ApolloUtils.apolloResponseToResult
    |> E.R.fmap(e => {
         let bots = e##bots |> unpackEdges |> E.A.fmap(toBot);
         bots;
       })
    |> E.R.fmap(fn)
    |> E.R.id
  )
  |> E.React.el;
};