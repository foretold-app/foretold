let toGlobalSetting = a =>
  Primary.GlobalSetting.make(~id=a##id, ~entityGraph=a##entityGraph, ());

module Query = [%graphql
  {|
    query globalSetting ($name: String) {
      globalSetting(name: $name) {
        id
        entityGraph
      }
    }
  |}
];

module QueryComponent = ReasonApollo.CreateQuery(Query);

let inner = innerComponentFn => {
  <QueryComponent>
    ...{({result}) =>
      result
      |> HttpResponse.fromApollo
      |> HttpResponse.fmap(e =>
           e##globalSetting |> E.O.fmap(toGlobalSetting)
         )
      |> HttpResponse.optionalToMissing
      |> innerComponentFn
    }
  </QueryComponent>;
};