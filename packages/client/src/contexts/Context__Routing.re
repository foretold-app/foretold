type channelId = string;
type seriesId = string;
type measurablesSearchString = string;
type agentId = string;

module AgentPage = {
  module SubPage = {
    type t =
      | AgentMeasurements
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
      | Measurables(Context__QueryParams.MeasurableIndex.query)
      | Measurable(string)
      | NewMeasurable
      | Members
      | InviteNewMember
      | Settings
      | NewSeries
      | Series(seriesId);

    let toTab = (subPage: t): tab =>
      switch (subPage) {
      | Measurables(_) => Measurables
      | Measurable(_) => Measurables
      | NewMeasurable => Measurables
      | NewSeries => Measurables
      | Series(_) => Measurables
      | Members => Members
      | InviteNewMember => Members
      | Settings => Options
      };

    let fromTab = (tab: tab): t =>
      switch (tab) {
      | Measurables => Measurables({state: None})
      | Members => Members
      | Options => Settings
      };
  };

  type t = {
    channelId,
    subPage: SubPage.t,
  };
};

let getChannelId = (channelId: string): string =>
  switch (channelId) {
  | "global" => ""
  | _ => channelId
  };

module Route = {
  type t =
    | Home
    | Privacy
    | Terms
    | AgentIndex
    | Redirect
    | Login
    | Profile
    | BotCreate
    | BotEdit(string)
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
    | ["privacy_policy"] => Privacy
    | ["terms_and_conditions"] => Terms
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
    | ["communities", "new"] => ChannelNew
    | ["communities"] => ChannelIndex
    | ["bots", "new"] => BotCreate
    | ["bots", id, "edit"] => BotEdit(id)
    | ["measurables", id, "edit"] => MeasurableEdit(id)
    | ["c"] => ChannelIndex
    | ["c", channelId] =>
      Channel({
        channelId: getChannelId(channelId),
        subPage:
          Measurables(
            url.search
            |> Context__QueryParams.MeasurableIndex.fromStringWithDefaults,
          ),
      })
    | ["c", channelId, "m", measurableId] =>
      Channel({
        channelId: getChannelId(channelId),
        subPage: Measurable(measurableId),
      })
    | ["c", channelId, "new"] => Channel({channelId, subPage: NewMeasurable})
    | ["c", channelId, "edit"] => Channel({channelId, subPage: Settings})
    | ["c", channelId, "members"] => Channel({channelId, subPage: Members})
    | ["c", channelId, "invite"] =>
      Channel({channelId, subPage: InviteNewMember})
    | ["c", channelId, "s", "new"] =>
      Channel({channelId, subPage: NewSeries})
    | ["c", channelId, "s", seriesId] =>
      Channel({channelId, subPage: Series(seriesId)})
    | ["agents", agentId] => Agent({agentId, subPage: AgentMeasurements})
    | ["agents", agentId, "bots"] => Agent({agentId, subPage: AgentBots})
    | ["agents", agentId, "measurables"] =>
      Agent({agentId, subPage: AgentMeasurables})
    | _ => NotFound
    };
};

module Url = {
  type t =
    | Home
    | Privacy
    | Terms
    | AgentIndex
    | Profile
    | EntityIndex
    | BotCreate
    | BotEdit(string)
    | EntityShow(string)
    | Agent(AgentPage.t)
    | ChannelShow(string)
    | ChannelNew
    | ChannelIndex
    | SeriesShow(string, string)
    | MeasurableShow(string, string)
    | SeriesNew(string)
    | MeasurableEdit(string)
    | ChannelEdit(string)
    | ChannelMembers(string)
    | ChannelInvite(string)
    | MeasurableNew(string);

  let toString = (r: t) =>
    switch ((r: t)) {
    | Home => "/"
    | Privacy => "/privacy_policy"
    | Terms => "/terms_and_conditions"
    | AgentIndex => "/agents"
    | Profile => "/profile/"
    | BotCreate => "/bots/new"
    | BotEdit(id) => "/bots/" ++ id ++ "/edit"
    | EntityIndex => "/entities"
    | EntityShow(id) => "/entities/" ++ id
    | Agent({agentId, subPage: AgentMeasurements}) => "/agents/" ++ agentId
    | Agent({agentId, subPage: AgentBots}) => "/agents/" ++ agentId ++ "/bots"
    | Agent({agentId, subPage: AgentMeasurables}) =>
      "/agents/" ++ agentId ++ "/measurables"
    | ChannelNew => "/communities/" ++ "new"
    | ChannelIndex => "/communities"
    | ChannelShow(id) => "/c/" ++ id
    | ChannelEdit(id) => "/c/" ++ id ++ "/edit"
    | ChannelMembers(id) => "/c/" ++ id ++ "/members"
    | ChannelInvite(channelId) => "/c/" ++ channelId ++ "/invite"
    | MeasurableEdit(id) => "/measurables/" ++ id ++ "/edit"
    | MeasurableNew(channelId) => "/c/" ++ channelId ++ "/new"
    | SeriesNew(channelId) => "/c/" ++ channelId ++ "/s/new"
    | SeriesShow(channelId, id) => "/c/" ++ channelId ++ "/s/" ++ id
    | MeasurableShow(channelId, id) => "/c/" ++ channelId ++ "/m/" ++ id
    };

  let push = (r: t) => r |> toString |> ReasonReact.Router.push;

  let fromChannelPage = (channelPage: ChannelPage.t) =>
    switch (channelPage.subPage) {
    | Measurables(_) => ChannelShow(channelPage.channelId)
    | NewMeasurable => MeasurableNew(channelPage.channelId)
    | Members => ChannelMembers(channelPage.channelId)
    | InviteNewMember => ChannelInvite(channelPage.channelId)
    | Settings => ChannelEdit(channelPage.channelId)
    | NewSeries => SeriesNew(channelPage.channelId)
    | Series(id) => SeriesShow(channelPage.channelId, id)
    | Measurable(id) => MeasurableShow(channelPage.channelId, id)
    };
};