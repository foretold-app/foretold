[@bs.config {jsx: 3}];

let appApolloClient = serverJwt => AppApolloClient.instance(serverJwt);

module Main = {
  [@react.component]
  let make = (~serverJwt) =>
    <ReasonApollo.Provider client={appApolloClient(serverJwt)}>
      {GlobalSettingGet.inner(globalSetting =>
         UserGet.inner(loggedUser =>
           <Providers.AppContext.Provider value={loggedUser, globalSetting}>
             <Navigator />
             <Intercom />
             <CheckSession />
           </Providers.AppContext.Provider>
         )
       )}
    </ReasonApollo.Provider>;
};

[@react.component]
let make = () => {
  let saveAuth0Info = _ => {
    let url = ReasonReact.Router.dangerouslyGetInitialUrl();
    let auth0TokensFromUrl = url |> Auth.UrlToAuth0Tokens.make;
    auth0TokensFromUrl |> E.O.fmap(Auth0Tokens.set) |> E.O.default();
  };

  let getServerJwt = fn => {
    let url = ReasonReact.Router.dangerouslyGetInitialUrl();
    let auth0Tokens = url |> Auth.UrlToAuth0Tokens.make;
    let foretoldAuthToken = url |> Auth.UrlToTokens.make;
    <ReasonApollo.Provider client={appApolloClient(None)}>
      {Authentication.component(auth0Tokens, foretoldAuthToken, fn)}
    </ReasonApollo.Provider>;
  };

  let serverJwt = ServerJwt.make_from_storage();

  switch (serverJwt) {
  | Some(_) => <Main serverJwt />
  | None =>
    saveAuth0Info();
    getServerJwt(serverJwt => {
      ServerJwt.set(serverJwt);
      <Main serverJwt />;
    });
  };
};
