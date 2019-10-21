open Routing;

let component = ReasonReact.statelessComponent("Navigator");

let make = (~route: Route.t, ~loggedInUser: option(Types.user), _children) => {
  ...component,
  render: _ => {
    switch (route, loggedInUser) {
    | (Home, Some(loggedInUser)) => Redirect.defaultPage(loggedInUser)

    | (Preferences, Some(loggedInUser)) =>
      <FillWithSidebar loggedInUser>
        <Preferences loggedInUser />
      </FillWithSidebar>
    | (ChannelNew, Some(loggedInUser)) =>
      <FillWithSidebar loggedInUser> <ChannelNew /> </FillWithSidebar>
    | (MeasurableEdit(id), Some(loggedInUser)) =>
      <FillWithSidebar loggedInUser>
        <MeasurableEdit loggedInUser pageParams={id: id} />
      </FillWithSidebar>
    | (BotCreate, Some(loggedInUser)) =>
      <FillWithSidebar loggedInUser>
        <BotCreate loggedInUser />
      </FillWithSidebar>
    | (BotEdit(botId), Some(loggedInUser)) =>
      <FillWithSidebar loggedInUser>
        <BotEdit pageParams={id: botId} loggedInUser />
      </FillWithSidebar>
    | (ChannelIndex, Some(loggedInUser)) =>
      <FillWithSidebar loggedInUser> <ChannelIndex.Jsx2 /> </FillWithSidebar>
    | (Profile, Some(loggedInUser)) =>
      <FillWithSidebar loggedInUser>
        <Profile loggedInUser />
      </FillWithSidebar>
    | (Subscribe, Some(loggedInUser)) =>
      <FillWithSidebar loggedInUser>
        <Preferences loggedInUser />
      </FillWithSidebar>
    | (Unsubscribe, Some(loggedInUser)) =>
      <FillWithSidebar loggedInUser>
        <Preferences loggedInUser />
      </FillWithSidebar>

    | (Channel(channel), _) =>
      <ChannelNavigation channelPage=channel loggedInUser />
    | (Agent(agentPage), _) => <Agent_Layout agentPage loggedInUser />
    | (AgentIndex, _) => <AgentIndex />
    | (EntityShow(id), Some(loggedInUser)) =>
      <FillWithSidebar loggedInUser>
        <EntityShow pageParams={id: id} />
      </FillWithSidebar>
    | (EntityIndex, Some(loggedInUser)) =>
      <FillWithSidebar loggedInUser> <EntityIndex /> </FillWithSidebar>
    | (Privacy, _) =>
      <StaticPageInCard markdown=StaticMarkdown.privacyPolicy />
    | (Terms, _) =>
      <StaticPageInCard markdown=StaticMarkdown.termsAndConditions />
    | (Login, _) => <Login />
    | (_, _) => <Home />
    };
  },
};