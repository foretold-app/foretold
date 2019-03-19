module Route = {
  type t =
    | Home
    | AgentIndex
    | Redirect
    | Login
    | Profile
    | ItemShow(string)
    | AgentShow(string)
    | AgentMeasurables(string)
    | ChannelShow(string)
    | ChannelIndex
    | ChannelNew
    | MeasurableShow(string, string)
    | MeasurableEdit(string)
    | MeasurableNew(string)
    | Series(string, string)
    | NotFound;

  let fromUrl = (url: ReasonReact.Router.url) =>
    switch (url.path) {
    | [] => Home
    | ["login"] => Login
    | ["callback"] =>
      Auth0.handleAuth(url);
      Redirect;
    | ["redirect"] => Redirect
    | ["agents"] => AgentIndex
    | ["profile"] =>
      switch (Auth0.userId()) {
      | Some(_) => Profile
      | None => Home
      }
    | ["agents", id] => AgentShow(id)
    | ["items", ...id] => ItemShow(String.concat("/", id))
    | ["agents", id, "measurables"] => AgentMeasurables(id)
    | ["channels", "new"] => ChannelNew
    | ["channels"] => ChannelIndex
    | ["c"] => ChannelIndex
    | ["c", id] => ChannelShow(id)
    | ["c", id, "new"] => MeasurableNew(id)
    | ["c", channel, "m", id] => MeasurableShow(channel, id)
    | ["measurables", id, "edit"] => MeasurableEdit(id)
    | ["c", channel, "s", id] => Series(channel, id)
    | _ => NotFound
    };
};

module Link = {
  type t =
    | Home
    | AgentIndex
    | Profile
    | ItemShow(string)
    | AgentShow(string)
    | AgentMeasurables(string)
    | ChannelShow(string)
    | ChannelNew
    | ChannelIndex
    | SeriesShow(string, string)
    | MeasurableShow(string, string)
    | MeasurableEdit(string)
    | MeasurableNew(string)
    | NotFound;

  let mapLinkToUrl = (r: t) =>
    switch ((r: t)) {
    | Home => "/"
    | AgentIndex => "/agents"
    | Profile => "/profile/"
    | ItemShow(id) => "/items/" ++ id
    | AgentShow(id) => "/agents/" ++ id
    | AgentMeasurables(id) => "/agents/" ++ id ++ "/measurables"
    | ChannelNew => "/channels/" ++ "new"
    | ChannelShow(id) => "/c/" ++ id
    | ChannelIndex => "/channels"
    | MeasurableShow(channel, id) => "/c/" ++ channel ++ "/m/" ++ id
    | MeasurableEdit(id) => "/measurables/" ++ id ++ "/edit"
    | MeasurableNew(channel) => "/c/" ++ channel ++ "/new"
    | SeriesShow(channel, id) => "/c/" ++ channel ++ "/s/" ++ id
    | _ => ""
    };
  let push = (r: t) => r |> mapLinkToUrl |> ReasonReact.Router.push;
};