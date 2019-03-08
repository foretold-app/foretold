open Routes;

type state = {route};
type action =
  | ChangeRoute(route);

let reducer = (action, _state) =>
  switch (action) {
  | ChangeRoute(route) => ReasonReact.Update({route: route})
  };

let mapUrlToRoute = (url: ReasonReact.Router.url) =>
  switch (url.path) {
  | [] => Home
  | ["callback"] =>
    Auth0.handleAuth(url);
    Redirect;
  | ["redirect"] => Redirect
  | ["agents"] => AgentIndex
  | ["profile"] =>
    switch (Auth0.userId()) {
    | Some(auth0Id) => Profile(auth0Id)
    | None => Home
    }
  | ["agents", id] => AgentShow(id)
  | ["items", ...id] => ItemShow(String.concat("/", id))
  | ["agents", id, "measurables"] => AgentMeasurables(id)
  | ["c", id] => Channel(id)
  | ["c", id, "new"] => MeasurableNew(id)
  | ["c", channel, "m", id] => MeasurableShow(channel, id)
  | ["measurables", id, "edit"] => MeasurableEdit(id)
  | _ => Home
  };

let mapUrlToAction = (url: ReasonReact.Router.url) =>
  ChangeRoute(url |> mapUrlToRoute);

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
  render: self => <TopLevel route={self.state.route} />,
};