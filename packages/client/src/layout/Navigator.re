open Routing;

let component = ReasonReact.statelessComponent("Navigator");

let make = (~route: Route.t, ~loggedInUser: option(Types.user), _children) => {
  ...component,
  render: _ => {
    switch (route, loggedInUser) {
    | (Home, Some(loggedInUser)) => Redirect.defaultPage(loggedInUser)

    | (Preferences, Some(loggedInUser)) => <Preferences loggedInUser />
    | (ChannelNew, Some(_)) => <ChannelNew />
    | (MeasurableEdit(id), Some(loggedInUser)) =>
      <MeasurableEdit loggedInUser pageParams={id: id} />
    | (BotCreate, Some(loggedInUser)) => <BotCreate loggedInUser />
    | (BotEdit(botId), Some(loggedInUser)) =>
      <BotEdit pageParams={id: botId} loggedInUser />
    | (ChannelIndex, Some(loggedInUser)) =>
      <FillWithSidebar loggedInUser> <ChannelIndex.Jsx2 /> </FillWithSidebar>
    | (Profile, Some(loggedInUser)) =>
      <FillWithSidebar loggedInUser>
        <Profile loggedInUser />
      </FillWithSidebar>
    | (Subscribe, Some(loggedInUser)) => <Preferences loggedInUser />
    | (Unsubscribe, Some(loggedInUser)) => <Preferences loggedInUser />

    | (Channel(channel), _) =>
      <ChannelNavigation channelPage=channel loggedInUser />
    | (Agent(agentPage), _) => <Agent_Layout agentPage loggedInUser />
    | (AgentIndex, _) => <AgentIndex />
    | (EntityShow(id), _) => <EntityShow pageParams={id: id} />
    | (EntityIndex, _) => <EntityIndex />
    | (Privacy, _) =>
      <StaticPageInCard markdown=StaticMarkdown.privacyPolicy />
    | (Terms, _) =>
      <StaticPageInCard markdown=StaticMarkdown.termsAndConditions />
    | (Login, _) => <Login />
    | (_, _) => <Home />
    };
  },
};