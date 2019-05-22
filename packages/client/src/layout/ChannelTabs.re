open Utils;

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

  type channel = {
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
    | Channel(channel)
    | ChannelIndex
    | ChannelNew
    | MeasurableEdit(string)
    | MeasurableNew(string)
    | Series(string, string)
    | SeriesNew(string)
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
    | ["c"] => ChannelIndex
    | ["measurables", id, "edit"] => MeasurableEdit(id)
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