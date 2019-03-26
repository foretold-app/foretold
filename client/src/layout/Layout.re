open Context.Routing;
open Foretold__GraphQL;
open Rationale.Function.Infix;

let defaultChannel = "4c5af8ad-1258-4b9c-8fa0-cc522708fc1c";

let tochannelId = (route: Route.t) =>
  switch (route) {
  | ChannelShow(c) => Some(c)
  | MeasurableNew(c) => Some(c)
  | Series(c, _) => Some(c)
  | Home => Some(defaultChannel)
  | _ => None
  };

let toRoutePage = (route: Route.t, me: Context.Me.me) =>
  switch (me) {
  | WithTokensAndUserData({userData}) =>
    let loggedInUser = userData;
    let channelId = tochannelId(route);
    let sidebar =
      Layout__Component__FillWithSidebar.make(~channelId, ~loggedInUser);

    let inApp = (~key="") =>
      E.React.makeToEl ||> E.React.withParent(~key, sidebar);

    switch (route) {
    | AgentMeasurables(id) =>
      AgentMeasurables.make(~id, ~loggedInUser) |> inApp
    | AgentIndex => AgentIndex.make |> inApp
    | EntityShow(id) => EntityShow.make(~id) |> inApp
    | Redirect => Redirect.make(~me) |> inApp
    | Profile => Profile.make(~loggedInUser) |> inApp
    | AgentShow(id) => AgentShow.make(~id) |> inApp
    | ChannelShow(channelId) =>
      MeasurableIndex.make(~channelId, ~loggedInUser)
      |> inApp(~key=channelId)
    | ChannelIndex => ChannelIndex.make(~loggedInUser) |> inApp
    | ChannelNew => ChannelNew.make |> inApp
    | MeasurableNew(channelId) => MeasurableNew.make(~channelId) |> inApp
    | MeasurableEdit(id) => MeasurableEdit.make(~id) |> inApp
    | Series(channelId, id) =>
      Queries.Channel.component(~id=channelId, channel =>
        SeriesShow.make(~id, ~channel, ~loggedInUser) |> inApp
      )
    | _ =>
      MeasurableIndex.make(~channelId=defaultChannel, ~loggedInUser)
      |> inApp(~key=defaultChannel)
    };
  | _ =>
    switch (route) {
    | Home => <Home />
    | Login => <Login />
    | Redirect => <Redirect me />
    | _ => <Home />
    }
  };

let make = (route: Route.t) =>
  route |> toRoutePage |> Queries.User.withLoggedInUserQuery;