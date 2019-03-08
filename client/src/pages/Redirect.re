open Utils;
open Rationale.Result.Infix;
open Rationale;
open E;

let ste = ReasonReact.string;

let component = ReasonReact.statelessComponent("Redirecting...");

let withUserQuery =
    (
      auth0Id: option(string),
      innerComponentFn: 'a => ReasonReact.reactElement,
    ) =>
  switch (auth0Id) {
  | Some(auth) =>
    let query = Queries.GetUser.Query.make(~auth0Id=auth, ());
    Queries.GetUser.QueryComponent.make(
      ~variables=query##variables, ~pollInterval=5000, ({result}) =>
      result
      |> ApolloUtils.apolloResponseToResult
      <$> (e => innerComponentFn(Some(e)))
      |> E.R.id
    )
    |> ReasonReact.element;
  | None => innerComponentFn(None)
  };

let make = _children => {
  ...component,
  render: _ =>
    withUserQuery(
      Auth0.userId(),
      userQuery => {
        let agentId =
          userQuery
          |> O.bind(_, e => e##user)
          |> O.bind(_, e => e##agent)
          |> O.fmap(e => e##id);
        let name =
          userQuery |> O.bind(_, e => e##user) |> O.fmap(e => e##name);
        switch (name, agentId) {
        | (Some(""), _) => Urls.pushToLink(Profile)
        | (_, Some(id)) => Urls.pushToLink(AgentShow(id))
        | _ => ()
        };
        <div>
          <h1> {"Redirecting..." |> ste} </h1>
          <p>
            {
              "If you are not redirected shortly, try refreshing the page or contacting Ozzie."
              |> ste
            }
          </p>
        </div>;
      },
    ),
};