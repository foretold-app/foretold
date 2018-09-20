%bs.raw
"require('../node_modules/handsontable/dist/handsontable.full.css');";

type route =
  | Home
  | Users
  | Agents
  | Agent(string)
  | User(string)
  | Measurables
  | Measurable(string)
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
  | ["users"] => Users
  | ["agents"] => Agents
  | ["agents", id] => Agent(id)
  | ["measurables"] => Measurables
  | ["users", id] => User(id)
  | ["measurables", id] => Measurable(id)
  | _ => Home
  };

let mapUrlToAction = (url: ReasonReact.Router.url) =>
  ChangeRoute(url |> mapUrlToRoute);

let component = ReasonReact.reducerComponent("App");

let inside = r =>
  switch (r) {
  | Home => <Users />
  | Agents => <Agents />
  | Agent(id) => <Agent id />
  | User(id) => <User id />
  | Users => <Users />
  | Measurables => <Measurables />
  | Measurable(id) => <Measurable id />
  | NotFound => <Home />
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
  render: self => <PaddedLayout> (self.state.route |> inside) </PaddedLayout>,
};