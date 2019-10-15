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
                      user {
                          id
                          name
                          description
                          picture
                          agentId
                      }
                      bot {
                          id
                          name
                          description
                          picture
                          competitorType
                          user {
                              id
                              name
                              description
                              picture
                              agentId
                          }
                      }
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

let toBot = bot => {
  let allowMutations =
    bot##permissions##mutations##allow |> E.A.O.concatSome |> E.A.to_list;

  let permissions = Primary.Permissions.make(allowMutations);

  let agentType =
    E.O.bind(
      bot##agent,
      Primary.AgentType.getAgentType(
        ~agent=_,
        ~getOwner=Primary.AgentType.getEmptyOwner,
        (),
      ),
    );

  let agent =
    bot##agent
    |> E.O.fmap(agent =>
         Primary.Agent.make(
           ~id=agent##id,
           ~agentType,
           ~name=Some(bot##name),
           (),
         )
       );

  Primary.Bot.make(
    ~id=bot##id,
    ~name=Some(bot##name),
    ~description=bot##description,
    ~competitorType=bot##competitorType,
    ~token=bot##token,
    ~agent,
    ~permissions=Some(permissions),
    ~picture=bot##picture,
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
