open Rationale.Result.Infix;

let withUserQuery =
    (
      auth0Id: option(string),
      innerComponentFn: 'a => ReasonReact.reactElement,
    ) =>
  switch (auth0Id) {
  | Some(auth) =>
    let query = GetUser.Query.make(~auth0Id=auth, ());
    GetUser.QueryComponent.make(~variables=query##variables, ({result}) =>
      result
      |> ApolloUtils.apolloResponseToResult
      <$> (e => innerComponentFn(Some(e)))
      |> E.R.id
    )
    |> ReasonReact.element;
  | None => innerComponentFn(None)
  };

let withLoggedInUserQuery = (innerComponentFn: 'a => ReasonReact.reactElement) => {
  let isLoggedIn = Auth0.isLoggedIn();
  isLoggedIn ?
    withUserQuery(Auth0.userId(), innerComponentFn) : innerComponentFn(None);
};