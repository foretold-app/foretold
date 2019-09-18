type channelId = string;
type seriesId = string;
type measurablesSearchString = string;
type agentId = string;

module AgentPage = {
  module SubPage = {
    type t =
      | AgentMeasurements
      | AgentMeasurables
      | AgentBots
      | AgentCommunities
      | AgentUpdates;
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
    | Options
    | Updates
    | Leaderboard;

  type leaderboard =
    | ByMeasurement
    | ByMeasurable
    | ByMember;

  type measurementsTab =
    | ByMeasurements
    | ByScores;

  module SubPage = {
    type t =
      | Measurables(MeasurableQueryIndex.query)
      | Measurable(string, measurementsTab)
      | NewMeasurable
      | Members
      | AddMember
      | InviteMember
      | Settings
      | NewSeries
      | Series(seriesId)
      | FeedItems
      | Leaderboard(leaderboard);

    let toTab = (subPage: t): tab =>
      switch (subPage) {
      | Measurables(_) => Measurables
      | Measurable(_) => Measurables
      | NewMeasurable => Measurables
      | NewSeries => Measurables
      | Series(_) => Measurables
      | Members => Members
      | AddMember => Members
      | InviteMember => Members
      | Settings => Options
      | FeedItems => Updates
      | Leaderboard(_) => Leaderboard
      };

    let fromTab = (tab: tab): t =>
      switch (tab) {
      | Measurables => Measurables({state: None})
      | Members => Members
      | Options => Settings
      | Updates => FeedItems
      | Leaderboard => Leaderboard(ByMember)
      };
  };

  type t = {
    channelId,
    subPage: SubPage.t,
  };
};

let getChannelId = (channelId: string): string =>
  switch (channelId) {
  | "home" => ""
  | _ => channelId
  };

module Route = {
  type t =
    | Home
    | Privacy
    | Terms
    | AgentIndex
    | Login
    | Profile
    | Preferences
    | BotCreate
    | BotEdit(string)
    | EntityShow(string)
    | EntityIndex
    | Channel(ChannelPage.t)
    | Agent(AgentPage.t)
    | ChannelIndex
    | ChannelNew
    | MeasurableEdit(string)
    | Subscribe
    | Unsubscribe
    | NotFound;

  let fromUrl = (url: ReasonReact.Router.url) =>
    switch (url.path) {
    | [] => Home
    | ["privacy_policy"] => Privacy
    | ["terms_and_conditions"] => Terms
    | ["login"] => Login
    | ["callback"] =>
      Auth.UrlToAuth0Tokens.make(url)
      |> E.O.fmap(Auth0Tokens.set)
      |> E.O.default();
      Home;
    | ["agents"] => AgentIndex
    | ["profile"] => Profile
    | ["preferences"] => Preferences
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
            url.search |> MeasurableQueryIndex.fromStringWithDefaults,
          ),
      })
    | ["c", channelId, "m", measurableId]
    | ["c", channelId, "m", measurableId, "questions"] =>
      Channel({
        channelId: getChannelId(channelId),
        subPage: Measurable(measurableId, ByMeasurements),
      })
    | ["c", channelId, "m", measurableId, "scores"] =>
      Channel({
        channelId: getChannelId(channelId),
        subPage: Measurable(measurableId, ByScores),
      })
    | ["c", channelId, "new"] => Channel({channelId, subPage: NewMeasurable})
    | ["c", channelId, "edit"] => Channel({channelId, subPage: Settings})
    | ["c", channelId, "members"] => Channel({channelId, subPage: Members})
    | ["c", channelId, "activity"] =>
      Channel({channelId: getChannelId(channelId), subPage: FeedItems})
    | ["c", channelId, "scoring", "predictions"] =>
      Channel({
        channelId: getChannelId(channelId),
        subPage: Leaderboard(ByMeasurement),
      })
    | ["c", channelId, "scoring", "questions"] =>
      Channel({
        channelId: getChannelId(channelId),
        subPage: Leaderboard(ByMeasurable),
      })
    | ["c", channelId, "scoring", "members"] =>
      Channel({
        channelId: getChannelId(channelId),
        subPage: Leaderboard(ByMember),
      })
    | ["c", channelId, "add"] => Channel({channelId, subPage: AddMember})
    | ["c", channelId, "invite"] =>
      Channel({channelId, subPage: InviteMember})
    | ["c", channelId, "s", "new"] =>
      Channel({channelId, subPage: NewSeries})
    | ["c", channelId, "s", seriesId] =>
      Channel({
        channelId: getChannelId(channelId),
        subPage: Series(seriesId),
      })
    | ["agents", agentId] => Agent({agentId, subPage: AgentMeasurements})
    | ["agents", agentId, "bots"] => Agent({agentId, subPage: AgentBots})
    | ["agents", agentId, "measurables"] =>
      Agent({agentId, subPage: AgentMeasurables})
    | ["agents", agentId, "communities"] =>
      Agent({agentId, subPage: AgentCommunities})
    | ["agents", agentId, "activity"] =>
      Agent({agentId, subPage: AgentUpdates})
    | ["subscribe"] => Subscribe
    | ["unsubscribe"] => Unsubscribe
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
    | Preferences
    | EntityIndex
    | BotCreate
    | BotEdit(string)
    | EntityShow(string)
    | Agent(AgentPage.t)
    | ChannelShow(string)
    | ChannelNew
    | ChannelIndex
    | SeriesShow(string, string)
    | MeasurableShow(string, string, ChannelPage.measurementsTab)
    | SeriesNew(string)
    | MeasurableEdit(string)
    | ChannelEdit(string)
    | ChannelMembers(string)
    | ChannelFeedItems(string)
    | ChannelLeaderboard(string, ChannelPage.leaderboard)
    | ChannelAddMember(string)
    | ChannelInviteMember(string)
    | MeasurableNew(string)
    | Subscribe
    | Login
    | Unsubscribe;

  let toString = (r: t) =>
    switch ((r: t)) {
    | Home => "/"
    | Privacy => "/privacy_policy"
    | Terms => "/terms_and_conditions"
    | AgentIndex => "/agents"
    | Profile => "/profile/"
    | Preferences => "/preferences/"
    | BotCreate => "/bots/new"
    | BotEdit(id) => "/bots/" ++ id ++ "/edit"
    | EntityIndex => "/entities"
    | EntityShow(id) => "/entities/" ++ id
    | Agent({agentId, subPage: AgentMeasurements}) => "/agents/" ++ agentId
    | Agent({agentId, subPage: AgentBots}) => "/agents/" ++ agentId ++ "/bots"
    | Agent({agentId, subPage: AgentMeasurables}) =>
      "/agents/" ++ agentId ++ "/measurables"
    | Agent({agentId, subPage: AgentCommunities}) =>
      "/agents/" ++ agentId ++ "/communities"
    | Agent({agentId, subPage: AgentUpdates}) =>
      "/agents/" ++ agentId ++ "/activity"
    | ChannelNew => "/communities/" ++ "new"
    | ChannelIndex => "/communities"
    | ChannelShow(channelId) => "/c/" ++ channelId
    | ChannelEdit(channelId) => "/c/" ++ channelId ++ "/edit"
    | ChannelMembers(channelId) => "/c/" ++ channelId ++ "/members"
    | ChannelFeedItems(channelId) => "/c/" ++ channelId ++ "/activity"
    | ChannelLeaderboard(channelId, ByMeasurement) =>
      "/c/" ++ channelId ++ "/scoring/predictions"
    | ChannelLeaderboard(channelId, ByMeasurable) =>
      "/c/" ++ channelId ++ "/scoring/questions"
    | ChannelLeaderboard(channelId, ByMember) =>
      "/c/" ++ channelId ++ "/scoring/members"
    | ChannelAddMember(channelId) => "/c/" ++ channelId ++ "/add"
    | ChannelInviteMember(channelId) => "/c/" ++ channelId ++ "/invite"
    | MeasurableEdit(measurableId) =>
      "/measurables/" ++ measurableId ++ "/edit"
    | MeasurableNew(channelId) => "/c/" ++ channelId ++ "/new"
    | SeriesNew(channelId) => "/c/" ++ channelId ++ "/s/new"
    | SeriesShow(channelId, id) => "/c/" ++ channelId ++ "/s/" ++ id
    | MeasurableShow(channelId, measurableId, ByMeasurements) =>
      "/c/" ++ channelId ++ "/m/" ++ measurableId ++ "/questions"
    | MeasurableShow(channelId, measurableId, ByScores) =>
      "/c/" ++ channelId ++ "/m/" ++ measurableId ++ "/scores"
    | Subscribe => "/subscribe"
    | Unsubscribe => "/unsubscribe"
    | Login => "/login"
    };

  let push = (r: t) => r |> toString |> ReasonReact.Router.push;

  let fromChannelPage = (channelPage: ChannelPage.t) =>
    switch (channelPage.subPage) {
    | Measurables(_) => ChannelShow(channelPage.channelId)
    | NewMeasurable => MeasurableNew(channelPage.channelId)
    | Members => ChannelMembers(channelPage.channelId)
    | FeedItems => ChannelFeedItems(channelPage.channelId)
    | Leaderboard(subTab) =>
      ChannelLeaderboard(channelPage.channelId, subTab)
    | AddMember => ChannelAddMember(channelPage.channelId)
    | InviteMember => ChannelInviteMember(channelPage.channelId)
    | Settings => ChannelEdit(channelPage.channelId)
    | NewSeries => SeriesNew(channelPage.channelId)
    | Series(id) => SeriesShow(channelPage.channelId, id)
    | Measurable(measurableId, measurementsTab) =>
      MeasurableShow(channelPage.channelId, measurableId, measurementsTab)
    };
};