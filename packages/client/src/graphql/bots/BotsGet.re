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
                  agent {
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

let toBot = botJson => {
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
  <QueryComponent variables=query##variables>
    ...{({result}) =>
      result
      |> ApolloUtils.apolloResponseToResult
      |> E.R.fmap(e => e##bots |> unpackEdges |> E.A.fmap(toBot))
      |> E.R.fmap(fn)
      |> E.R.id
    }
  </QueryComponent>;
};
