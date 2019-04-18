[@bs.val] [@bs.scope ("window", "location")]
external reload: unit => unit = "reload";

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

let component = (auth0Tokens: Context.Auth.Auth0Tokens.t, _innerComponent) => {
  Js.log("Checking for jwt token");
  switch (Context.Auth.ServerJwt.make_from_storage()) {
  | Some(_) => _innerComponent
  | None =>
    let query = Query.make(~auth0jwt=auth0Tokens.id_token, ());
    Js.log("Getting jwt token from Server");
    QueryComponent.make(~variables=query##variables, ({result}) =>
      result
      |> E.HttpResponse.fromApollo
      |> E.HttpResponse.fmap(e => e##authentication##jwt)
      |> (
        e =>
          switch (e) {
          | Success(c) =>
            Context.Auth.ServerJwt.set(c);
            reload();
            _innerComponent;
          | _ => redirectingMessage
          }
      )
    )
    |> E.React.el;
  };
};