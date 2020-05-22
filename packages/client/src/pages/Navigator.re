[@react.component]
let make = () => {
  // @todo: Do not set URL as string directly,
  // @todo: push new state into the application state.
  // @todo: keywords: application state
  let (route, setRoute) =
    React.useState(() =>
      ReasonReact.Router.dangerouslyGetInitialUrl() |> Routing.Route.fromUrl
    );

  React.useState(() => {
    History.onPushState2((event: History.event2) => {
      Js.log2("event1 onPushState2", event##state);
      Js.log2("event1 onPushState2", History.getHistoryState());
      let url = ReasonReact.Router.dangerouslyGetInitialUrl();
      ();
      setRoute(_ => url |> Routing.Route.fromUrl);
    });
    History.onPopState2((event: History.event2) => {
      Js.log2("event2 onPopState2", event##state);
      Js.log2("event2 onPopState2", History.getHistoryState());

      let url = ReasonReact.Router.dangerouslyGetInitialUrl();
      setRoute(_ => url |> Routing.Route.fromUrl);
      ();
    });
  })
  |> ignore;

  let context = React.useContext(Providers.app);

  <>
    {switch (route, context.loggedUser) {
     // Static Pages
     | (Privacy, _) =>
       <StaticPageInCard markdown=StaticMarkdown.privacyPolicy />
     | (Terms, _) =>
       <StaticPageInCard markdown=StaticMarkdown.termsAndConditions />
     | (Login, _) => <Login />

     // Channels
     | (Channel(channelPage), _) => <ChannelNavigator channelPage />
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
    <Title route />
    <Redirect route loggedUser={context.loggedUser} />
  </>;
};