type channelId = string;
type seriesId = string;
type measurablesSearchString = string;
type agentId = string;

module AgentPage = {
  module SubPage = {
    type t =
      | AgentShow
      | AgentMeasurables
      | AgentBots;
  };

  type t = {
    agentId,
    subPage: SubPage.t,
  };
};

module ChannelPage = {
  type tab =
    | Measurables
    | Members
    | Options;

  module SubPage = {
    type t =
      | Measurables(measurablesSearchString)
      | NewMeasurable
      | Members
      | InviteNewMember
      | Settings
      | NewSeries
      | Series(seriesId);

    let toTab = (t: t): tab =>
      switch (t) {
      | Measurables(_) => Measurables
      | NewMeasurable => Measurables
      | Members => Members
      | InviteNewMember => Members
      | Settings => Options
      | NewSeries => Measurables
      | Series(_) => Measurables
      };

    let fromTab = (tab: tab): t =>
      switch (tab) {
      | Measurables => Measurables("")
      | Members => Members
      | Options => Settings
      };
  };

  type t = {
    channelId,
    subPage: SubPage.t,
  };
};

module Route = {
  type t =
    | Home
    | AgentIndex
    | Redirect
    | Login
    | Profile
    | BotCreate
    | EntityShow(string)
    | EntityIndex
    | Channel(ChannelPage.t)
    | Agent(AgentPage.t)
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
    | ["agents", agentId] => Agent({agentId, subPage: AgentShow})
    | ["agents", agentId, "bots"] => Agent({agentId, subPage: AgentBots})
    | ["agents", agentId, "measurables"] =>
      Agent({agentId, subPage: AgentMeasurables})
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
    | Agent(AgentPage.t)
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
    | Agent({agentId, subPage: AgentShow}) => "/agents/" ++ agentId
    | Agent({agentId, subPage: AgentBots}) => "/agents/" ++ agentId ++ "/bots"
    | Agent({agentId, subPage: AgentMeasurables}) =>
      "/agents/" ++ agentId ++ "/measurables"
    | ChannelNew => "/channels/" ++ "new"
    | ChannelIndex => "/channels"
    | ChannelShow(id) => "/c/" ++ id
    | ChannelEdit(id) => "/c/" ++ id ++ "/edit"
    | ChannelMembers(id) => "/c/" ++ id ++ "/members"
    | ChannelInvite(channel) => "/c/" ++ channel ++ "/invite"
    | MeasurableEdit(id) => "/measurables/" ++ id ++ "/edit"
    | MeasurableNew(channel) => "/c/" ++ channel ++ "/new"
    | SeriesNew(channel) => "/c/" ++ channel ++ "/s/new"
    | SeriesShow(channel, id) => "/c/" ++ channel ++ "/s/" ++ id
    };

  let push = (r: t) => r |> toString |> ReasonReact.Router.push;

  let fromChannelPage = (t: ChannelPage.t) =>
    switch (t.subPage) {
    | Measurables(_) => ChannelShow(t.channelId)
    | NewMeasurable => MeasurableNew(t.channelId)
    | Members => ChannelMembers(t.channelId)
    | InviteNewMember => ChannelInvite(t.channelId)
    | Settings => ChannelEdit(t.channelId)
    | NewSeries => SeriesNew(t.channelId)
    | Series(id) => SeriesShow(t.channelId, id)
    };
};