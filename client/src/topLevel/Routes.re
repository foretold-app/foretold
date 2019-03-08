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