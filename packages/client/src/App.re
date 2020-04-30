let appApolloClient = serverJwt => AppApolloClient.instance(serverJwt);

module Main = {
  [@react.component]
  let make = (~serverJwt) => {
    let client = appApolloClient(serverJwt);
    <ReasonApollo.Provider client>
      <ReasonApolloHooks.ApolloProvider client>
        {GlobalSettingGet.inner(globalSetting =>
           UserGet.inner(loggedUser => {
             let value: Providers.appContext = {loggedUser, globalSetting};
             <Providers.AppContexProvider value>
               <Navigator />
               <Intercom />
             </Providers.AppContexProvider>;
           })
         )}
      </ReasonApolloHooks.ApolloProvider>
    </ReasonApollo.Provider>;
  };
};

[@react.component]
let make = () => {
  let url = ReasonReact.Router.dangerouslyGetInitialUrl();

  let clearUrl = (url: ReasonReact.Router.url) => {
    let remove = [|
      "access_token",
      "scope",
      "expires_in",
      "token_type",
      "state",
      "token",
      "id_token",
    |];
    KeyValuePairs.clearHash(url, remove) |> ReasonReact.Router.replace;
  };

  let saveAuth0Info = url => {
    let auth0TokensFromUrl = url |> Auth.UrlToAuth0Tokens.make;
    auth0TokensFromUrl |> E.O.fmap(Auth0Tokens.set) |> E.O.default();
  };

  let getServerJwt = (url, fn) => {
    let auth0Tokens = url |> Auth.UrlToAuth0Tokens.make;
    let foretoldAuthToken = url |> Auth.UrlToTokens.make;
    <ReasonApollo.Provider client={appApolloClient(None)}>
      {Authentication.component(auth0Tokens, foretoldAuthToken, fn)}
    </ReasonApollo.Provider>;
  };

  let serverJwt = ServerJwt.make_from_storage();

  clearUrl(url);
  saveAuth0Info(url);

  switch (serverJwt) {
  | Some(_) => <Main serverJwt />
  | None =>
    getServerJwt(
      url,
      serverJwt => {
        ServerJwt.set(serverJwt);
        <Main serverJwt />;
      },
    )
  };
};