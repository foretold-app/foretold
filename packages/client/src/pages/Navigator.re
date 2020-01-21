[@react.component]
let make = () => {
  let (route, setRoute) =
    React.useState(() =>
      ReasonReact.Router.dangerouslyGetInitialUrl() |> Routing.Route.fromUrl
    );

  React.useState(() => {
    History.onPushState(event =>
      if (!History.miss(event)) {
        let url = ReasonReact.Router.dangerouslyGetInitialUrl();
        setRoute(_ => url |> Routing.Route.fromUrl);
      }
    );
    History.onPopState(_ => {
      let url = ReasonReact.Router.dangerouslyGetInitialUrl();
      setRoute(_ => url |> Routing.Route.fromUrl);
    });
  })
  |> ignore;

  route
  |> Routing.Route.title
  |> E.Title.toString
  |> ForetoldComponents.Hooks.useTitle;

  let context = React.useContext(Providers.app);

  <>
    {switch (route, context.loggedUser) {
     // Statis Pages
     | (Privacy, _) =>
       <StaticPageInCard markdown=StaticMarkdown.privacyPolicy />
     | (Terms, _) =>
       <StaticPageInCard markdown=StaticMarkdown.termsAndConditions />
     | (Login, _) => <Login />

     // Channels
     | (Channel(channel), _) => <ChannelNavigator channelPage=channel />
     | (ChannelIndex, _) =>
       <FillWithSidebar> <ChannelIndex /> </FillWithSidebar>
     | (ChannelNew, Some(_)) =>
       <FillWithSidebar> <ChannelNew /> </FillWithSidebar>

     // Members
     | (Agent(agentPage), _) => <AgentNavigator agentPage />
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
     }}
    <Redirect route loggedUser={context.loggedUser} />
  </>;
};