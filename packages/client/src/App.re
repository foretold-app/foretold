let component = ReasonReact.statelessComponent("App");
let appApolloClient = AppApolloClient.instance();

let make = _children => {
  ...component,
  render: _ => {
    let getUser = fn => {
      let serverJwt = ServerJwt.make_from_storage();
      let auth0tokens = Auth0Tokens.make_from_storage();
      let authToken =
        ReasonReact.Router.dangerouslyGetInitialUrl() |> Auth.UrlToTokens.make;

      switch (serverJwt, authToken, auth0tokens) {
      | (Some(_), _, _) => UserGet.component(fn)
      | (_, _, _) => Authentication.component(auth0tokens, authToken)
      };
    };

    <ReasonApollo.Provider client=appApolloClient>
      {GlobalSettingGet.component(globalSetting =>
         getUser(loggedInUser => {
           Js.log2("loggedInUser", loggedInUser);

           let appContext: Providers.appContext = {
             loggedInUser,
             globalSetting,
           };

           <Providers.AppContext.Provider value=appContext>
             <Router />
           </Providers.AppContext.Provider>;
         })
       )}
    </ReasonApollo.Provider>;
  },
};
