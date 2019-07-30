type state = {
  route: Routing.Route.t,
  authToken: option(string),
  routingCount: int,
};

type action =
  | ChangeRoute(Routing.Route.t)
  | ChangeAuthToken(string);

let mapUrlToAction = (url: ReasonReact.Router.url) =>
  ChangeRoute(url |> Routing.Route.fromUrl);

let urlToRoute = (url: ReasonReact.Router.url, send) =>
  url |> mapUrlToAction |> send;

let tokenToState = (url: ReasonReact.Router.url, send) => {
  let token = url |> Auth.UrlToTokens.make;
  switch (token) {
  | Some(s) =>
    KeyValuePairs.clearHash(url, "token") |> ReasonReact.Router.replace;
    send(ChangeAuthToken(s));
  | _ => ()
  };
};

let component = ReasonReact.reducerComponent("App");
let appApolloClient = AppApolloClient.instance();

let make = _children => {
  ...component,
  reducer: (action, state) =>
    switch (action) {
    | ChangeRoute(route) =>
      ReasonReact.Update({
        ...state,
        route,
        routingCount: state.routingCount + 1,
      })
    | ChangeAuthToken(authToken) =>
      ReasonReact.Update({...state, authToken: Some(authToken)})
    },

  initialState: () => {route: Home, authToken: None, routingCount: 0},

  didMount: self => {
    let initUrl = ReasonReact.Router.dangerouslyGetInitialUrl();

    // @todo: use one step of state updating
    urlToRoute(initUrl, self.send);
    tokenToState(initUrl, self.send);

    let watcherID =
      ReasonReact.Router.watchUrl(url => {
        urlToRoute(url, self.send);
        ();
      });

    self.onUnmount(() => ReasonReact.Router.unwatchUrl(watcherID));
  },

  render: self => {
    let state: state = self.state;

    let meToUser = (me: option(Me.t)) =>
      switch (me) {
      | Some(WithTokensAndUserData({userData})) => Some(userData)
      | _ => None
      };

    let getUser = innerComponentFn => {
      let serverJwt = ServerJwt.make_from_storage();
      let auth0tokens = Auth0Tokens.make_from_storage();
      let authToken = state.authToken;

      switch (serverJwt, authToken, auth0tokens) {
      | (Some(_), _, _) => UserGet.inner(innerComponentFn)
      | (_, None, None) => innerComponentFn(Me.WithoutTokens)
      | (_, _, _) =>
        Authentication.component(
          auth0tokens,
          authToken,
          UserGet.inner(innerComponentFn),
        )
      };
    };

    <ReasonApollo.Provider client=appApolloClient>
      {getUser((me: Me.t) => {
         let loggedInUser = meToUser(Some(me));

         let appContext: Providers.appContext = {
           route: state.route,
           authToken: state.authToken,
           me: Some(me),
           loggedInUser,
           routingCount: state.routingCount,
         };

         <Providers.AppContext.Provider value=appContext>
           <Navigator route={self.state.route} loggedInUser />
           <Redirect />
         </Providers.AppContext.Provider>;
       })}
    </ReasonApollo.Provider>;
  },
};