type state = {
  route: Routing.Route.t,
  authToken: option(string),
};

type action =
  | ChangeState(state);

let mapUrlToAction = (state, url: ReasonReact.Router.url) => {
  let route = url |> Routing.Route.fromUrl;
  ChangeState({...state, route});
};

let firstStateUpdate = (_state, url: ReasonReact.Router.url) => {
  let route = url |> Routing.Route.fromUrl;
  let authToken = url |> Auth.UrlToTokens.make;
  // @todo: remove side effect
  //  KeyValuePairs.clearHash(url, "token") |> ReasonReact.Router.replace;
  ChangeState({authToken, route});
};

let component = ReasonReact.reducerComponent("App");
let appApolloClient = AppApolloClient.instance();

let make = _children => {
  ...component,
  reducer: (action, _state) =>
    switch (action) {
    | ChangeState(state) => ReasonReact.Update(state)
    },

  initialState: () => {route: Home, authToken: None},

  didMount: self => {
    let initUrl = ReasonReact.Router.dangerouslyGetInitialUrl();

    firstStateUpdate(self.state, initUrl) |> self.send;

    let watcherID =
      ReasonReact.Router.watchUrl(url => {
        mapUrlToAction(self.state, url) |> self.send;
        ();
      });

    self.onUnmount(() => ReasonReact.Router.unwatchUrl(watcherID));
  },

  render: self => {
    let state = self.state;

    let getUser = innerComponentFn => {
      let serverJwt = ServerJwt.make_from_storage();
      let auth0tokens = Auth0Tokens.make_from_storage();
      let authToken = state.authToken;

      switch (serverJwt, authToken, auth0tokens) {
      | (Some(_), _, _) => UserGet.inner(innerComponentFn)
      | (_, None, None) => innerComponentFn(None)
      | (_, _, _) => Authentication.component(auth0tokens, authToken)
      };
    };

    <ReasonApollo.Provider client=appApolloClient>
      {GlobalSettingGet.inner((globalSetting: option(Types.globalSetting)) =>
         getUser((loggedInUser: option(Types.user)) => {
           let appContext: Providers.appContext = {
             route: state.route,
             authToken: state.authToken,
             loggedInUser,
             globalSetting,
           };

           <Providers.AppContext.Provider value=appContext>
             <Navigator route={state.route} loggedInUser />
             <Redirect appContext />
           </Providers.AppContext.Provider>;
         })
       )}
    </ReasonApollo.Provider>;
  },
};
