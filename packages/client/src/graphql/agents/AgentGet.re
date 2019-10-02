type user = {
  id: string,
  name: string,
  description: option(string),
  score: option(float),
};

type bot = {
  competitorType: Types.competitorType,
  description: option(string),
  id: string,
  name: string,
};

type agent = {
  user: option(user),
  bot: option(bot),
  isMe: bool,
};

module Query = [%graphql
  {|
    query getAgent ($id: String!) {
        agent(id: $id) @bsRecord{
            isMe
            user @bsRecord{
              id
              name
              description
              score
            }
            bot @bsRecord{
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

type response = {agent};

let component = (~id, innerFn) => {
  open Utils;

  let notFound = "Agent not found" |> ste;
  let query = Query.make(~id, ());

  <QueryComponent variables=query##variables>
    ...{({result}) =>
      result
      |> ApolloUtils.apolloResponseToResult
      |> E.R.fmap(e => {
           let agent = e##agent;

           switch (agent) {
           | Some(a) => Some({agent: a})
           | _ => None
           };
         })
      |> E.R.bind(_, e =>
           switch (e) {
           | Some(a) => Ok(a)
           | None => Error(notFound |> E.React.inH3)
           }
         )
      |> E.R.fmap(innerFn)
      |> E.R.id
    }
  </QueryComponent>;
};
