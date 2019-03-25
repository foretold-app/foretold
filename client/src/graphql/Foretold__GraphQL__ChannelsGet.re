open Utils;

module Query = [%graphql
  {|
    query getChannels {
      channels{
        id
        name
        description
        isArchived
        isPublic
      }
    }
  |}
];

module QueryComponent = ReasonApollo.CreateQuery(Query);

let toChannel = m: Context.Primary.Channel.t =>
  Context.Primary.Channel.make(
    ~id=m##id,
    ~name=m##name,
    ~description=m##description,
    ~isArchived=m##isArchived,
    ~isPublic=m##isPublic,
    (),
  );

let component = fn => {
  let query = Query.make();
  QueryComponent.make(~variables=query##variables, ({result}) =>
    result
    |> ApolloUtils.apolloResponseToResult
    |> E.R.fmap(e => e##channels |> E.A.O.concatSomes |> E.A.fmap(toChannel))
    |> E.R.fmap(fn)
    |> E.R.id
  )
  |> E.React.el;
};