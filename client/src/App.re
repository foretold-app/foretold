type route =
  | Home
  | AgentIndex
  | Redirect
  | Profile(string)
  | ItemShow(string)
  | AgentShow(string)
  | AgentMeasurables(string)
  | Channel(string)
  | MeasurableShow(string, string)
  | MeasurableEdit(string)
  | MeasurableNew(string)
  | NotFound;

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

let inside = r =>
  switch (r) {
  | Home => <MeasurableIndex channel="general" />
  | AgentMeasurables(id) => <MeMeasurables id />
  | AgentIndex => <AgentIndex />
  | NotFound => <MeasurableIndex channel="general" />
  | ItemShow(id) => <ItemShow id />
  | Redirect => <Redirect />
  | Profile(auth0Id) => <Profile auth0Id />
  | AgentShow(id) => <AgentShow id />
  | Channel(channel) => <MeasurableIndex channel />
  | MeasurableNew(channel) => <MeasurableNew channel />
  | MeasurableShow(channel, id) => <MeasurableShow channel id />
  | MeasurableEdit(id) => <MeasurableEdit id />
  };

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
  render: self => self.state.route |> inside,
};