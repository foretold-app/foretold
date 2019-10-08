type state = {route: Routing.Route.t};

type action =
  | ChangeState(state);

let mapUrlToAction = (url: ReasonReact.Router.url) => {
  let route = url |> Routing.Route.fromUrl;
  ChangeState({route: route});
};

let component = ReasonReact.reducerComponent("Router");
let make = _children => {
  ...component,
  reducer: (action, _state) =>
    switch (action) {
    | ChangeState(state) => ReasonReact.Update(state)
    },

  initialState: () => {route: Home},

  didMount: self => {
    let initUrl = ReasonReact.Router.dangerouslyGetInitialUrl();

    mapUrlToAction(initUrl) |> self.send;

    let watcherID =
      ReasonReact.Router.watchUrl(url => {
        mapUrlToAction(url) |> self.send;
        ();
      });

    self.onUnmount(() => ReasonReact.Router.unwatchUrl(watcherID));
  },

  render: self => {
    <Providers.AppContext.Consumer>
      ...{appContext =>
        <>
          <Navigator
            route={self.state.route}
            loggedInUser={appContext.loggedInUser}
          />
          <Redirect
            route={self.state.route}
            loggedInUser={appContext.loggedInUser}
          />
        </>
      }
    </Providers.AppContext.Consumer>;
  },
};
