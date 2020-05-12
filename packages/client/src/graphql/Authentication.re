[@bs.val] [@bs.scope ("window", "location")]
external reload: unit => unit = "reload";

module Query = [%graphql
  {|
    query authentication (
        $auth0jwt: JWT
        $auth0accessToken: String
        $authToken: String
    ) {
        authentication (
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

// @todo: To make a component.
let redirectingMessage =
  <h3> {"You are being redirected..." |> Utils.ste} </h3>;

let component = (auth0Tokens, authToken, fn) => {
  let auth0jwt =
    auth0Tokens
    |> E.O.fmap((r: Auth0Tokens.t) => Some(Js.Json.string(r.id_token)))
    |> E.O.default(None);

  let auth0accessToken =
    auth0Tokens
    |> E.O.fmap((r: Auth0Tokens.t) => Some(r.access_token))
    |> E.O.default(None);

  switch (auth0Tokens, authToken) {
  | (None, None) => fn(None)
  | (_, _) =>
    let query = Query.make(~auth0jwt?, ~auth0accessToken?, ~authToken?, ());
    <QueryComponent variables=query##variables>
      ...{({result}) =>
        result
        |> HttpResponse.fromApollo
        |> HttpResponse.fmap(e => e##authentication##jwt)
        |> (
          e =>
            switch (e) {
            | Success(c) => fn(Some(c))
            | Error(_) => fn(None)
            | _ => redirectingMessage
            }
        )
      }
    </QueryComponent>;
  };
};