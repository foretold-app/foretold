[@bs.config {jsx: 3}];

[@react.component]
let make = () =>
  <AgentsGet.QueryComponent>
    ...{({result}) =>
      result
      |> ApolloUtils.apolloResponseToResult
      |> E.R.fmap(d => d##agents)
      |> E.R.fmap(_ => E.React2.null)
      |> E.R.id
    }
  </AgentsGet.QueryComponent>;
