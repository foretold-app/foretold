open Utils;

module Query = [%graphql
  {|
    query bots {
      bots(first: 200){
          edges{
              node{
                  id
                  name
                  description
                  competitorType
                  jwt
              }
          }
      }
    }
  |}
];

module QueryComponent = ReasonApollo.CreateQuery(Query);

let unpackEdges = a: array('a) => {
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
  "competitorType": [ | `AGGREGATION | `COMPETITIVE | `OBJECTIVE],
  "id": string,
  "description": option(string),
  "jwt": option(string),
  "name": option(string),
};

let toBot = (m: bot) =>
  Context.Primary.Bot.make(
    ~id=m##id,
    ~name=m##name,
    ~description=m##description,
    ~competitorType=m##competitorType,
    ~jwt=m##jwt,
    (),
  );

let component = fn => {
  let query = Query.make();
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