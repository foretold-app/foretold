module Query = [%graphql
  {|
    query authentication ($auth0jwt: String!) {
        authentication(auth0jwt: $auth0jwt) {
            jwt
        }
    }
  |}
];

module QueryComponent = ReasonApollo.CreateQuery(Query);

let withLoggedInUserQuery = innerComponentFn =>
  switch (Context.Auth.Auth0Tokens.make_from_storage()) {
  | None =>
    Context.Auth.Actions.logout();
    innerComponentFn();
  | Some(tokens) =>
    Context.Auth.Actions.logoutIfTokenIsObsolete(tokens);
    let query = Query.make(~auth0jwt=tokens.id_token, ());
    QueryComponent.make(~variables=query##variables, ({result}) =>
      result
      |> E.HttpResponse.fromApollo
      |> E.HttpResponse.fmap(e => e##authentication##jwt)
      |> (
        e =>
          switch (e) {
          | Success(c) => innerComponentFn()
          | _ =>
            Context.Auth.Actions.logout();
            innerComponentFn();
          }
      )
    )
    |> E.React.el;
  };