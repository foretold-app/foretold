open Routes;

type state = {route: Route.t};
type action =
  | ChangeRoute(Route.t);

let reducer = (action, _state) =>
  switch (action) {
  | ChangeRoute(route) => ReasonReact.Update({route: route})
  };

let mapUrlToAction = (url: ReasonReact.Router.url) =>
  ChangeRoute(url |> Routes.Route.fromUrl);

let component = ReasonReact.reducerComponent("App");

let make = _children => {
  ...component,
  reducer,
  initialState: () => {route: Home},
  didMount: self => {
    ReasonReact.Router.dangerouslyGetInitialUrl()
    |> mapUrlToAction
    |> self.send;
    let watcherID =
      ReasonReact.Router.watchUrl(url => url |> mapUrlToAction |> self.send);
    self.onUnmount(() => ReasonReact.Router.unwatchUrl(watcherID));
  },
  render: self => <Layout route={self.state.route} />,
};