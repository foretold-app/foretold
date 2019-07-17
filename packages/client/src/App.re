type state = {
  route: Routing.Route.t,
  authToken: option(string),
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

let component = "App" |> ReasonReact.reducerComponent;
let appApolloClient = AppApolloClient.instance();

let make = _children => {
  ...component,
  reducer: (action, state) =>
    switch (action) {
    | ChangeRoute(route) => ReasonReact.Update({...state, route})
    | ChangeAuthToken(authToken) =>
      ReasonReact.Update({...state, authToken: Some(authToken)})
    },

  initialState: () => {route: Home, authToken: None},

  didMount: self => {
    let initUrl = ReasonReact.Router.dangerouslyGetInitialUrl();
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
    let value: Providers.appContext = {authToken: state.authToken};

    <ReasonApollo.Provider client=appApolloClient>
      <Providers.AppContext.Provider value>
        {Layout.make(self.state.route)}
      </Providers.AppContext.Provider>
    </ReasonApollo.Provider>;
  },
};