[@bs.config {jsx: 3}];

let appApolloClient = AppApolloClient.instance();

let a1 =
  React.useState(() => {
    let url = ReasonReact.Router.dangerouslyGetInitialUrl();
    url |> Routing.Route.fromUrl;
  });

[@react.component]
let make = () => {
  let (route, setRoute) = a1;

  ReasonReact.Router.watchUrl(url => {
    setRoute(_ => url |> Routing.Route.fromUrl);
    ();
  })
  |> ignore;

  let getUser = fn => {
    let serverJwt = ServerJwt.make_from_storage();
    let auth0tokens = Auth0Tokens.make_from_storage();

    let authToken =
      ReasonReact.Router.dangerouslyGetInitialUrl() |> Auth.UrlToTokens.make;

    switch (serverJwt, authToken, auth0tokens) {
    | (Some(_), _, _) => UserGet.inner(fn)
    | (_, None, None) => fn(None)
    | (_, _, _) => Authentication.component(auth0tokens, authToken)
    };
  };

  <ReasonApollo.Provider client=appApolloClient>
    {"0" |> ReasonReact.string}
  </ReasonApollo.Provider>;
};