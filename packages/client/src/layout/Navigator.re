[@bs.config {jsx: 3}];

[@react.component]
let make = () => {
  let (route, setRoute) =
    React.useState(() => {
      let url = ReasonReact.Router.dangerouslyGetInitialUrl();
      url |> Routing.Route.fromUrl;
    });

  ReasonReact.Router.watchUrl(url => {
    setRoute(_ => url |> Routing.Route.fromUrl);
    ();
  })
  |> ignore;

  <Providers.AppContext.Consumer>
    ...{({loggedUser}) => {
      let routeComponent =
        switch (route, loggedUser) {
        | (Privacy, _) =>
          <StaticPageInCard markdown=StaticMarkdown.privacyPolicy />
        | (Terms, _) =>
          <StaticPageInCard markdown=StaticMarkdown.termsAndConditions />
        | (Login, _) => <Login />

        | (Channel(channel), _) => <ChannelNavigation channelPage=channel />
        | (ChannelIndex, _) =>
          <FillWithSidebar> <ChannelIndex /> </FillWithSidebar>
        | (ChannelNew, Some(_)) =>
          <FillWithSidebar> <ChannelNew /> </FillWithSidebar>

        | (Agent(agentPage), _) => <AgentNavigation agentPage />
        | (AgentIndex, _) => <AgentIndex />

        | (Preferences, Some(loggedUser)) =>
          <FillWithSidebar> <Preferences loggedUser /> </FillWithSidebar>
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
          <FillWithSidebar>
            <EntityShow pageParams={id: id} />
          </FillWithSidebar>
        | (EntityIndex, Some(_)) =>
          <FillWithSidebar> <EntityIndex /> </FillWithSidebar>

        | (_, _) => <Home />
        };

      <> routeComponent <Redirect route loggedUser /> </>;
    }}
  </Providers.AppContext.Consumer>;
};