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
    | (Agent(agentPage), _) => <AgentNavigation agentPage />
    | (AgentIndex, _) => <AgentIndex />

    | (Home, Some(loggedUser)) => Redirect.defaultPage(loggedUser)
    | (Preferences, Some(loggedUser)) =>
      <FillWithSidebar> <Preferences loggedUser /> </FillWithSidebar>
    | (ChannelNew, Some(_)) =>
      <FillWithSidebar> <ChannelNew /> </FillWithSidebar>
    | (MeasurableEdit(id), Some(loggedUser)) =>
      <FillWithSidebar>
        <MeasurableEdit loggedUser pageParams={id: id} />
      </FillWithSidebar>
    | (BotCreate, Some(loggedUser)) =>
      <FillWithSidebar> <BotCreate loggedUser /> </FillWithSidebar>
    | (BotEdit(botId), Some(loggedUser)) =>
      <FillWithSidebar>
        <BotEdit pageParams={id: botId} loggedUser />
      </FillWithSidebar>
    | (ChannelIndex, Some(_)) =>
      <FillWithSidebar> <ChannelIndex.Jsx2 /> </FillWithSidebar>
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

    | (_, _) => "<Home /> Natigator" |> Utils.ste
    };
  },
};
