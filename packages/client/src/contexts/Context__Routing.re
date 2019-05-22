module Route = {
  type seriesId = string;
  type channelId = string;
  type measurablesSearchString = string;

  type channelSubPage =
    | Measurables(measurablesSearchString)
    | NewMeasurable
    | Members
    | InviteNewMember
    | Settings
    | NewSeries
    | Series(seriesId);

  type channelPage = {
    channelId,
    subPage: channelSubPage,
  };

  type t =
    | Home
    | AgentIndex
    | Redirect
    | Login
    | Profile
    | BotCreate
    | EntityShow(string)
    | EntityIndex
    | AgentShow(string)
    | AgentMeasurables(string)
    | AgentBots(string)
    | Channel(channelPage)
    | ChannelIndex
    | ChannelNew
    | MeasurableEdit(string)
    | NotFound;

  let fromUrl = (url: ReasonReact.Router.url) =>
    switch (url.path) {
    | [] => Home
    | ["login"] => Login
    | ["callback"] =>
      Context__Auth.CallbackUrlToAuth0Tokens.make(url)
      |> E.O.fmap(Context__Auth.Auth0Tokens.set)
      |> E.O.default();
      Redirect;
    | ["redirect"] => Redirect
    | ["agents"] => AgentIndex
    | ["profile"] => Profile
    | ["agents", id] => AgentShow(id)
    | ["agents", id, "bots"] => AgentBots(id)
    | ["agents", id, "measurables"] => AgentMeasurables(id)
    | ["entities"] => EntityIndex
    | ["entities", ...id] => EntityShow(String.concat("/", id))
    | ["channels", "new"] => ChannelNew
    | ["channels"] => ChannelIndex
    | ["bots", "new"] => BotCreate
    | ["measurables", id, "edit"] => MeasurableEdit(id)
    | ["c"] => ChannelIndex
    | ["c", channelId] =>
      Channel({channelId, subPage: Measurables(url.search)})
    | ["c", channelId, "new"] => Channel({channelId, subPage: NewMeasurable})
    | ["c", channelId, "edit"] => Channel({channelId, subPage: Settings})
    | ["c", channelId, "members"] => Channel({channelId, subPage: Members})
    | ["c", channelId, "s", "new"] =>
      Channel({channelId, subPage: NewSeries})
    | ["c", channelId, "s", seriesId] =>
      Channel({channelId, subPage: Series(seriesId)})
    | _ => NotFound
    };
};

module Url = {
  type t =
    | Home
    | AgentIndex
    | Profile
    | EntityIndex
    | BotCreate
    | EntityShow(string)
    | AgentShow(string)
    | AgentMeasurables(string)
    | AgentBots(string)
    | ChannelShow(string)
    | ChannelNew
    | ChannelIndex
    | SeriesShow(string, string)
    | SeriesNew(string)
    | MeasurableEdit(string)
    | ChannelEdit(string)
    | ChannelMembers(string)
    | ChannelInvite(string)
    | MeasurableNew(string);

  let toString = (r: t) =>
    switch ((r: t)) {
    | Home => "/"
    | AgentIndex => "/agents"
    | Profile => "/profile/"
    | BotCreate => "/bots/new"
    | EntityIndex => "/entities"
    | EntityShow(id) => "/entities/" ++ id
    | AgentShow(id) => "/agents/" ++ id
    | AgentBots(id) => "/agents/" ++ id ++ "/bots"
    | AgentMeasurables(id) => "/agents/" ++ id ++ "/measurables"
    | ChannelNew => "/channels/" ++ "new"
    | ChannelShow(id) => "/c/" ++ id
    | ChannelIndex => "/channels"
    | ChannelEdit(id) => "/c/" ++ id ++ "/edit"
    | ChannelMembers(id) => "/c/" ++ id ++ "/members"
    | ChannelInvite(channel) => "/c/" ++ channel ++ "/invite"
    | MeasurableEdit(id) => "/measurables/" ++ id ++ "/edit"
    | MeasurableNew(channel) => "/c/" ++ channel ++ "/new"
    | SeriesNew(channel) => "/c/" ++ channel ++ "/s/new"
    | SeriesShow(channel, id) => "/c/" ++ channel ++ "/s/" ++ id
    };

  let push = (r: t) => r |> toString |> ReasonReact.Router.push;
};