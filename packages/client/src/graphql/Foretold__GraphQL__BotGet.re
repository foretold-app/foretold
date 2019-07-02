type bot = {
  id: string,
  name: string,
  competitorType: Primary.CompetitorType.t,
  description: option(string),
};

let toBot = (bot: bot): Primary.Bot.t =>
  Primary.Bot.make(
    ~id=bot.id,
    ~name=Some(bot.name),
    ~competitorType=bot.competitorType,
    ~description=bot.description,
    (),
  );

module Query = [%graphql
  {|
      query getBot ($id: String!) {
          bot: bot(id: $id) @bsRecord {
           id
           name
           description
           competitorType
          }
      }
    |}
];

module QueryComponent = ReasonApollo.CreateQuery(Query);

let component = (~id, innerFn) => {
  let query = Query.make(~id, ());
  QueryComponent.make(~variables=query##variables, ({result}) =>
    result
    |> ApolloUtils.apolloResponseToResult
    |> Rationale.Result.fmap(e => e##bot |> Rationale.Option.fmap(toBot))
    |> Rationale.Result.fmap(innerFn)
    |> E.R.id
  )
  |> E.React.el;
};