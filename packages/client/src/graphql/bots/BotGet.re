let toBot = (bot): Types.bot =>
  Primary.Bot.make(
    ~id=bot##id,
    ~name=Some(bot##name),
    ~competitorType=bot##competitorType,
    ~description=bot##description,
    ~picture=bot##picture,
    (),
  );

module Query = [%graphql
  {|
      query bot ($id: String!) {
          bot(id: $id) {
           id
           name
           description
           competitorType
           picture
          }
      }
    |}
];

module QueryComponent = ReasonApollo.CreateQuery(Query);

let component = (~id, innerFn) => {
  let query = Query.make(~id, ());
  <QueryComponent variables=query##variables>
    ...{({result}) =>
      result
      |> ApolloUtils.apolloResponseToResult
      |> E.R.fmap(e => e##bot |> Rationale.Option.fmap(toBot))
      |> E.R.fmap(innerFn)
      |> E.R.id
    }
  </QueryComponent>;
};