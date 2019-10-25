[@bs.config {jsx: 3}];

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

let appApolloClient = AppApolloClient.instance();

[@react.component]
let make = () => {
  //  reducer: (action, _state) =>
  //    switch (action) {
  //    | ChangeState(state) => ReasonReact.Update(state)
  //    },
  //
  //
  //  didMount: self => {
  //    let initUrl = ReasonReact.Router.dangerouslyGetInitialUrl();
  //
  //    firstStateUpdate(self.state, initUrl) |> self.send;
  //
  //    let watcherID =
  //      ReasonReact.Router.watchUrl(url => {
  //        mapUrlToAction(self.state, url) |> self.send;
  //        ();
  //      });
  //
  //    self.onUnmount(() => ReasonReact.Router.unwatchUrl(watcherID));
  //  },

  // @todo: 1
  let state = {route: Home, authToken: None};

  let getUser = fn => {
    let serverJwt = ServerJwt.make_from_storage();
    let auth0tokens = Auth0Tokens.make_from_storage();
    let authToken = state.authToken;

    switch (serverJwt, authToken, auth0tokens) {
    | (Some(_), _, _) => UserGet.inner(fn)
    | (_, None, None) => fn(None)
    | (_, _, _) => Authentication.component(auth0tokens, authToken)
    };
  };

  <ReasonApollo.Provider client=appApolloClient>
    {GlobalSettingGet.inner((globalSetting: option(Types.globalSetting)) =>
       getUser((loggedUser: option(Types.user)) => {
         let appContext: Providers.appContext = {
           route: state.route,
           authToken: state.authToken,
           loggedUser,
           globalSetting,
         };

         <Providers.AppContext.Provider value=appContext>
           <Navigator route={state.route} loggedUser />
           <Redirect appContext />
           <Intercom />
           <CheckSession />
         </Providers.AppContext.Provider>;
       })
     )}
  </ReasonApollo.Provider>;
};
