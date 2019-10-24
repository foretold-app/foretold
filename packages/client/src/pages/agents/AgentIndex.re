[@bs.config {jsx: 3}];

[@react.component]
let make = () =>
  AgentsGet.QueryComponent.make(({result}) =>
    result
    |> ApolloUtils.apolloResponseToResult
    |> E.R.fmap(d => d##agents)
    |> E.R.fmap(_ => E.React.null)
    |> E.R.id
  )
  |> E.React.el;
