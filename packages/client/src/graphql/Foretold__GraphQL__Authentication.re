[@bs.val] [@bs.scope ("window", "location")]
external reload: unit => unit = "reload";

module Query = [%graphql
  {|
    query authentication (
        $auth0jwt: String!
        $auth0accessToken: String!
        $authToken: String!
    ) {
        authentication(
            auth0jwt: $auth0jwt
            auth0accessToken: $auth0accessToken
            authToken: $authToken
        ) {
            jwt
        }
    }
  |}
];

module QueryComponent = ReasonApollo.CreateQuery(Query);

let redirectingMessage =
  <h3> {"You are being redirected..." |> Utils.ste} </h3>;

let component =
    (
      auth0Tokens: option(Auth0Tokens.t),
      authToken: option(string),
      _innerComponent,
    ) => {
  let auth0jwt =
    auth0Tokens
    |> E.O.fmap((r: Auth0Tokens.t) => r.id_token)
    |> E.O.default("");

  let auth0accessToken =
    auth0Tokens
    |> E.O.fmap((r: Auth0Tokens.t) => r.access_token)
    |> E.O.default("");

  let authToken = authToken |> E.O.default("");

  let query = Query.make(~auth0jwt, ~auth0accessToken, ~authToken, ());

  QueryComponent.make(~variables=query##variables, ({result}) =>
    result
    |> E.HttpResponse.fromApollo
    |> E.HttpResponse.fmap(e => e##authentication##jwt)
    |> (
      e =>
        switch (e) {
        | Success(c) =>
          ServerJwt.set(c);
          reload();
          _innerComponent;
        | _ => redirectingMessage
        }
    )
  )
  |> E.React.el;
};