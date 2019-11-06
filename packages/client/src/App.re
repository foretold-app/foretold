[@bs.config {jsx: 3}];

let appApolloClient = AppApolloClient.instance();

[@react.component]
let make = () => {
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
    {GlobalSettingGet.inner((globalSetting: option(Types.globalSetting)) =>
       getUser((loggedUser: option(Types.user)) => {
         let value: Providers.appContext = {loggedUser, globalSetting};

         <Providers.AppContext.Provider value>
           <Navigator />
           <Intercom />
           <CheckSession />
         </Providers.AppContext.Provider>;
       })
     )}
  </ReasonApollo.Provider>;
};