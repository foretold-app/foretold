[@bs.config {jsx: 3}];

open Routing;

[@react.component]
let make = (~route: Route.t, ~loggedUser: option(Types.user)) => {
  switch (route, loggedUser) {
  | (Privacy, _) => <StaticPageInCard markdown=StaticMarkdown.privacyPolicy />
  | (Terms, _) =>
    <StaticPageInCard markdown=StaticMarkdown.termsAndConditions />
  | (Login, _) => <Login />

  | (Channel(channel), _) => <ChannelNavigation channelPage=channel />
  | (Agent(agentPage), _) => <AgentNavigation agentPage />
  | (AgentIndex, _) => <AgentIndex />
  | (ChannelIndex, _) =>
    <FillWithSidebar> <ChannelIndex.Jsx2 /> </FillWithSidebar>

  | (Preferences, Some(loggedUser)) =>
    <FillWithSidebar> <Preferences loggedUser /> </FillWithSidebar>
  | (ChannelNew, Some(_)) =>
    <FillWithSidebar> <ChannelNew /> </FillWithSidebar>
  | (MeasurableEdit(id), Some(loggedUser)) =>
    <FillWithSidebar>
      <MeasurableEdit pageParams={id: id} />
    </FillWithSidebar>
  | (BotCreate, Some(loggedUser)) =>
    <FillWithSidebar> <BotCreate loggedUser /> </FillWithSidebar>
  | (BotEdit(botId), Some(loggedUser)) =>
    <FillWithSidebar>
      <BotEdit pageParams={id: botId} loggedUser />
    </FillWithSidebar>
  | (Profile, Some(loggedUser)) =>
    <FillWithSidebar> <Profile loggedUser /> </FillWithSidebar>
  | (Subscribe, Some(loggedUser)) =>
    <FillWithSidebar> <Preferences loggedUser /> </FillWithSidebar>
  | (Unsubscribe, Some(loggedUser)) =>
    <FillWithSidebar> <Preferences loggedUser /> </FillWithSidebar>
  | (EntityShow(id), Some(_)) =>
    <FillWithSidebar> <EntityShow pageParams={id: id} /> </FillWithSidebar>
  | (EntityIndex, Some(_)) =>
    <FillWithSidebar> <EntityIndex /> </FillWithSidebar>

  | (_, _) => <Home />
  };
};
