open Context.Routing;
open Foretold__GraphQL;
open Rationale.Function.Infix;

let defaultChannel = "4c5af8ad-1258-4b9c-8fa0-cc522708fc1c";

let defaultPage = (loggedInUser: Context.Primary.User.t, layout) => {
  let firstUserChannel =
    loggedInUser.agent
    |> E.O.bind(_, (a: Context.Primary.Agent.t) => a.channelMemberships)
    |> E.A.O.defaultEmpty
    |> E.A.get(_, 0)
    |> E.O.bind(_, (r: Context.Primary.Types.channelMembership) => r.channel);

  let sidebar =
    Layout__Component__FillWithSidebar.make(
      ~channelId=
        firstUserChannel
        |> E.O.fmap((c: Context.Primary.Types.channel) => c.id),
      ~loggedInUser,
    );

  let inApp = (~key="") =>
    E.React.makeToEl ||> E.React.withParent(~key, sidebar);

  /* This should always be Some */
  switch (firstUserChannel) {
  | Some({id: channelIdSome}) =>
    Channel_Layout.makeWithPage(ChannelShow(channelIdSome), loggedInUser)
  | _ => ChannelIndex.make(~loggedInUser, ~layout) |> inApp
  };
};

let toRoutePage = (route: Route.t, me: Context.Me.me) =>
  switch (me) {
  | WithTokensAndUserData({userData}) =>
    let loggedInUser = userData;

    let sidebar =
      Layout__Component__FillWithSidebar.make(~channelId=None, ~loggedInUser);

    let inApp = (~key="") =>
      E.React.makeToEl ||> E.React.withParent(~key, sidebar);

    let layout = SLayout.FullPage.makeWithEl;

    switch (route) {
    | ChannelShow(_)
    | ChannelInvite(_)
    | ChannelMembers(_)
    | MeasurableNew(_)
    | SeriesNew(_)
    | Series(_, _)
    | ChannelEdit(_) => Channel_Layout.makeWithPage(route, loggedInUser)
    | AgentMeasurables(id) =>
      AgentMeasurables.make(~id, ~loggedInUser, ~layout) |> inApp
    | BotCreate => BotCreate.make(~layout) |> inApp
    | AgentIndex => AgentIndex.make(~layout) |> inApp
    | EntityShow(id) => EntityShow.make(~id, ~layout) |> inApp
    | Redirect => Auth0Redirect.make(~me) |> inApp
    | EntityIndex => EntityIndex.make(~layout) |> inApp
    | Profile => Profile.make(~loggedInUser, ~layout) |> inApp
    | AgentShow(id) => AgentShow.make(~id, ~layout) |> inApp
    | AgentBots(id) => AgentBots.make(~id, ~layout) |> inApp
    | ChannelIndex => ChannelIndex.make(~loggedInUser, ~layout) |> inApp
    | ChannelNew => ChannelNew.make(~layout) |> inApp
    | MeasurableEdit(id) => MeasurableEdit.make(~id, ~layout) |> inApp
    | _ => defaultPage(loggedInUser, layout)
    };
  | _ =>
    switch (route) {
    | Home => <Home />
    | Login => <Login />
    | Redirect => <Auth0Redirect me />
    | _ => <Home />
    }
  };

let make = (route: Route.t) =>
  toRoutePage(route) |> Queries.User.withLoggedInUserQuery;