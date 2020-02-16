let toGlobalSetting = a =>
  Primary.GlobalSetting.make(
    ~id=a##id,
    ~entityGraph=Some(KenTools.Graph.fromJson(a##entityGraph)),
    (),
  );

module Query = [%graphql
  {|
    query globalSetting ($name: String!) {
      globalSetting (name: $name) {
        id
        entityGraph
      }
    }
  |}
];

module QueryComponent = ReasonApollo.CreateQuery(Query);

let inner = fn => {
  let query = Query.make(~name="main", ());
  <QueryComponent variables=query##variables>
    ...{({result}) =>
      result
      |> ApolloUtils.apolloResponseToResult
      |> E.R.fmap(e => e##globalSetting |> E.O.fmap(toGlobalSetting))
      |> E.R.fmap(fn)
      |> E.R.id
    }
  </QueryComponent>;
};