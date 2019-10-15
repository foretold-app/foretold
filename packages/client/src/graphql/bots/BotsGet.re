module Query = [%graphql
  {|
    query bots ($ownerId: String) {
      bots(first: 200, ownerId: $ownerId){
          edges{
              node{
                  id
                  name
                  picture
                  description
                  competitorType
                  token
                  agent {
                    id
                    name
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
  a
  |> E.O.fmap(b => b##edges |> E.A.O.defaultEmpty |> E.A.O.concatSome)
  |> E.A.O.defaultEmpty
  |> E.A.fmap(e => e##node)
  |> E.A.O.concatSome;
};

let toBot = botJson => {
  let allowMutations =
    botJson##permissions##mutations##allow |> E.A.O.concatSome |> E.A.to_list;

  let permissions = Primary.Permissions.make(allowMutations);

  let agent =
    botJson##agent
    |> E.O.fmap(agentJson =>
         Primary.Agent.make(~id=agentJson##id, ~name=Some(botJson##name), ())
       );

  Primary.Bot.make(
    ~id=botJson##id,
    ~name=Some(botJson##name),
    ~description=botJson##description,
    ~competitorType=botJson##competitorType,
    ~token=botJson##token,
    ~agent,
    ~permissions=Some(permissions),
    ~picture=botJson##picture,
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
