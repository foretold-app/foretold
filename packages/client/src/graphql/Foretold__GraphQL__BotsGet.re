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
  "competitorType": Context.Primary.CompetitorType.t,
  "id": string,
  "description": option(string),
  "token": option(string),
  "agent": option({. "id": string}),
  "name": string,
};

let toBot = (m: bot) =>
  Context.Primary.Bot.make(
    ~id=m##id,
    ~name=Some(m##name),
    ~description=m##description,
    ~competitorType=m##competitorType,
    ~token=m##token,
    ~agent=
      m##agent |> E.O.fmap(r => Context.Primary.Agent.make(~id=r##id, ())),
    (),
  );

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