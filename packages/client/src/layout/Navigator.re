open Routing;

let component = ReasonReact.statelessComponent("Navigator");

let make = (~route: Route.t, ~loggedUser: option(Types.user), _children) => {
  ...component,
  render: _ => {
    switch (route, loggedUser) {
    | (Privacy, _) =>
      <StaticPageInCard markdown=StaticMarkdown.privacyPolicy />
    | (Terms, _) =>
      <StaticPageInCard markdown=StaticMarkdown.termsAndConditions />
    | (Login, _) => <Login />
    | (Channel(channel), _) =>
      <ChannelNavigation channelPage=channel loggedUser />
    | (Agent(agentPage), _) => <Agent_Layout agentPage loggedUser />
    | (AgentIndex, _) => <AgentIndex />

    | (Home, Some(loggedUser)) => Redirect.defaultPage(loggedUser)
    | (Preferences, Some(loggedUser)) =>
      <FillWithSidebar loggedUser={Some(loggedUser)}>
        <Preferences loggedUser />
      </FillWithSidebar>
    | (ChannelNew, Some(loggedUser)) =>
      <FillWithSidebar loggedUser={Some(loggedUser)}>
        <ChannelNew />
      </FillWithSidebar>
    | (MeasurableEdit(id), Some(loggedUser)) =>
      <FillWithSidebar loggedUser={Some(loggedUser)}>
        <MeasurableEdit loggedUser pageParams={id: id} />
      </FillWithSidebar>
    | (BotCreate, Some(loggedUser)) =>
      <FillWithSidebar loggedUser={Some(loggedUser)}>
        <BotCreate loggedUser />
      </FillWithSidebar>
    | (BotEdit(botId), Some(loggedUser)) =>
      <FillWithSidebar loggedUser={Some(loggedUser)}>
        <BotEdit pageParams={id: botId} loggedUser />
      </FillWithSidebar>
    | (ChannelIndex, Some(loggedUser)) =>
      <FillWithSidebar loggedUser={Some(loggedUser)}>
        <ChannelIndex.Jsx2 />
      </FillWithSidebar>
    | (Profile, Some(loggedUser)) =>
      <FillWithSidebar loggedUser={Some(loggedUser)}>
        <Profile loggedUser />
      </FillWithSidebar>
    | (Subscribe, Some(loggedUser)) =>
      <FillWithSidebar loggedUser={Some(loggedUser)}>
        <Preferences loggedUser />
      </FillWithSidebar>
    | (Unsubscribe, Some(loggedUser)) =>
      <FillWithSidebar loggedUser={Some(loggedUser)}>
        <Preferences loggedUser />
      </FillWithSidebar>
    | (EntityShow(id), Some(loggedUser)) =>
      <FillWithSidebar loggedUser={Some(loggedUser)}>
        <EntityShow pageParams={id: id} />
      </FillWithSidebar>
    | (EntityIndex, Some(loggedUser)) =>
      <FillWithSidebar loggedUser={Some(loggedUser)}>
        <EntityIndex />
      </FillWithSidebar>

    | (_, _) => <Home />
    };
  },
};