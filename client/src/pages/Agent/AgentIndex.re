open Utils;
open Foretold__GraphQL;

/* We currently don't use this file */
let component = ReasonReact.statelessComponent("Measurables");
let make = _children => {
  ...component,
  render: _ =>
    Queries.Agents.QueryComponent.make(({result}) =>
      result
      |> ApolloUtils.apolloResponseToResult
      |> E.R.fmap(d => d##agents)
      |> E.R.fmap(_ => E.React.null)
      |> E.R.id
    )
    |> E.React.el,
};