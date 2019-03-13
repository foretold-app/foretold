open Routes;

type link =
  | Home
  | AgentIndex
  | Profile
  | ItemShow(string)
  | AgentShow(string)
  | AgentMeasurables(string)
  | ChannelShow(string)
  | ChannelNew
  | ChannelIndex
  | MeasurableShow(string, string)
  | MeasurableEdit(string)
  | MeasurableNew(string)
  | NotFound;

let mapLinkToUrl = (r: link) =>
  switch ((r: link)) {
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
  | _ => ""
  };

let pushToLink = (r: link) => r |> mapLinkToUrl |> ReasonReact.Router.push;