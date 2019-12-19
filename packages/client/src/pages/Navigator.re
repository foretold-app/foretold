[@react.component]
let make = () => {
  let (route, setRoute) =
    React.useState(() =>
      ReasonReact.Router.dangerouslyGetInitialUrl() |> Routing.Route.fromUrl
    );

  ReasonReact.Router.watchUrl(url => {
    setRoute(_ => url |> Routing.Route.fromUrl);
    ();
  })
  |> ignore;

  <Providers.AppContext.Consumer>
    ...{({loggedUser}) => {
      let routeComponent =
        switch (route, loggedUser) {
        // Statis Pages
        | (Privacy, _) =>
          <StaticPageInCard markdown=StaticMarkdown.privacyPolicy />
        | (Terms, _) =>
          <StaticPageInCard markdown=StaticMarkdown.termsAndConditions />
        | (Login, _) => <Login />

        // Channels
        | (Channel(channel), _) => <ChannelNavigation channelPage=channel />
        | (ChannelIndex, _) =>
          <FillWithSidebar> <ChannelIndex /> </FillWithSidebar>
        | (ChannelNew, Some(_)) =>
          <FillWithSidebar> <ChannelNew /> </FillWithSidebar>

        // Members
        | (Agent(agentPage), _) => <AgentNavigation agentPage />
        | (AgentIndex, _) => <AgentIndex />
        | (Profile, Some(loggedUser)) =>
          <FillWithSidebar> <ProfileEdit loggedUser /> </FillWithSidebar>
        | (Subscribe, Some(loggedUser)) =>
          <FillWithSidebar> <SubscribePage loggedUser /> </FillWithSidebar>
        | (Unsubscribe, Some(loggedUser)) =>
          <FillWithSidebar> <UnsubscribePage loggedUser /> </FillWithSidebar>
        | (Preferences, Some(loggedUser)) =>
          <FillWithSidebar> <Preferences loggedUser /> </FillWithSidebar>

        // Bots
        | (BotCreate, Some(loggedUser)) =>
          <FillWithSidebar> <BotCreate loggedUser /> </FillWithSidebar>
        | (BotEdit(botId), Some(loggedUser)) =>
          <FillWithSidebar>
            <BotEdit pageParams={id: botId} loggedUser />
          </FillWithSidebar>

        // Entities
        | (EntityShow(entityId), Some(_)) =>
          <FillWithSidebar>
            <EntityShow pageParams={id: entityId} />
          </FillWithSidebar>
        | (EntityIndex, Some(_)) =>
          <FillWithSidebar> <EntityIndex /> </FillWithSidebar>

        | (_, _) => <Home />
        };

      <> routeComponent <Redirect route loggedUser /> </>;
    }}
  </Providers.AppContext.Consumer>;
};