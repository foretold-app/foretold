open Context.Routing;

type state = {
  route: Route.t,
  authToken: option(string),
};

type action =
  | ChangeRoute(Route.t)
  | ChangeAuthToken(string);

let reducer = (action, state) =>
  switch (action) {
  | ChangeRoute(route) => ReasonReact.Update({...state, route})
  | ChangeAuthToken(authToken) =>
    ReasonReact.Update({...state, authToken: Some(authToken)})
  };

let mapUrlToAction = (url: ReasonReact.Router.url) =>
  ChangeRoute(url |> Route.fromUrl);

type appContext = {authToken: option(string)};

module AppContextProvider =
  Contexts.MakePair({
    type t = appContext;
    let defaultValue = {authToken: None};
  });

//module ThemedButton = {
//  let component = "ThemedButton" |> ReasonReact.statelessComponent;
//
//  let make = _children => {
//    ...component,
//    render: _self =>
//      <AppContextProvider.Consumer>
//        ...{context =>
//          <div> {context.authToken |> E.O.default("s") |> Utils.ste} </div>
//        }
//      </AppContextProvider.Consumer>,
//  };
//};

let make = (componentForRoute, _children) => {
  let component = "App" |> ReasonReact.reducerComponent;
  {
    ...component,
    reducer,
    initialState: () => {route: Home, authToken: None},
    didMount: self => {
      let initUrl = ReasonReact.Router.dangerouslyGetInitialUrl();

      initUrl |> mapUrlToAction |> self.send;

      initUrl
      |> Auth.CallbackUrlToTokens.make
      |> E.O.fmap((r: Tokens.t) => self.send(ChangeAuthToken(r.token)))
      |> ignore;

      let watcherID =
        ReasonReact.Router.watchUrl(url => {
          url
          |> Auth.CallbackUrlToTokens.make
          |> E.O.fmap((r: Tokens.t) => self.send(ChangeAuthToken(r.token)))
          |> ignore;

          url |> mapUrlToAction |> self.send;
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
};