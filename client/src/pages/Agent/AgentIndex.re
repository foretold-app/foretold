open Utils;

let toAgentLink = (id, name) => {j|<a href="/agents/$id">$name</a>|j};

let component = ReasonReact.statelessComponent("Measurables");
let make = _children => {
  ...component,
  render: _ =>
    GetAgents.QueryComponent.make(({result}) =>
      result
      |> ApolloUtils.apolloResponseToResult
      |> E.R.fmap(d => d##agents)
      |> E.R.fmap(_ => E.React.null)
      |> E.R.id
    )
    |> E.React.el,
};