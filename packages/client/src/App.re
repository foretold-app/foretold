type state = {
  route: Routing.Route.t,
  authToken: option(string),
};

type action =
  | ChangeRoute(Routing.Route.t)
  | ChangeAuthToken(string);

let reducer = (action, state) =>
  switch (action) {
  | ChangeRoute(route) => ReasonReact.Update({...state, route})
  | ChangeAuthToken(authToken) =>
    ReasonReact.Update({...state, authToken: Some(authToken)})
  };

let mapUrlToAction = (url: ReasonReact.Router.url) =>
  ChangeRoute(url |> Routing.Route.fromUrl);

type appContext = {authToken: option(string)};

module AppContextProvider =
  Contexts.MakePair({
    type t = appContext;
    let defaultValue = {authToken: None};
  });

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

let make = (componentForRoute, _children) => {
  ...component,
  reducer,
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
    let value = {authToken: state.authToken};

    <AppContextProvider.Provider value>
      {self.state.route |> componentForRoute}
    </AppContextProvider.Provider>;
  },
};