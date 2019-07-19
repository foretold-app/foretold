/* We currently don't use this file */
let component = ReasonReact.statelessComponent("AgentIndex");
let make = (~layout=SLayout.FullPage.makeWithEl, _children) => {
  ...component,
  render: _ =>
    AgentsGet.QueryComponent.make(({result}) =>
      result
      |> ApolloUtils.apolloResponseToResult
      |> E.R.fmap(d => d##agents)
      |> E.R.fmap(_ => E.React.null)
      |> E.R.id
    )
    |> E.React.el,
};