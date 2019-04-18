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

let redirectingMessage =
  <h3> {"You are being redirected..." |> Utils.ste} </h3>;

let component = innerComponent =>
  switch (Context.Auth.Auth0Tokens.make_from_storage()) {
  | None =>
    Context.Auth.Actions.logout();
    redirectingMessage;
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
          | Success(c) =>
            Context.Auth.ServerJwt.set(c);
            ReasonReact.Router.push("/");
            redirectingMessage;
          | _ =>
            Context.Auth.Actions.logout();
            redirectingMessage;
          }
      )
    )
    |> E.React.el;
  };