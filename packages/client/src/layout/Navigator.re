open Routing;
open Pages;

let defaultPage = (loggedInUser: Primary.User.t) =>
  loggedInUser.agent
  |> E.O.bind(_, Primary.Agent.firstChannel)
  |> E.O.fmap((channel: Types.channel) => {
       Routing.Url.push(ChannelShow(channel.id));
       ReasonReact.null;
     })
  |> E.O.default(<Home />);

let meToUser = (me: Me.t) =>
  switch (me) {
  | WithTokensAndUserData({userData}) => Some(userData)
  | _ => None
  };

type pageParams = {id: string};

let toRoutePage = (route: Route.t, me: Me.t) => {
  let loggedInUser = meToUser(me);

  switch (route, loggedInUser) {
  | (Home, Some(loggedInUser)) => defaultPage(loggedInUser)
  | (Channel(channel), _) =>
    Channel_Layout.makeWithPage(channel, loggedInUser)
  | (Agent(agentPage), _) =>
    Agent_Layout.makeWithPage(agentPage, loggedInUser)
  | (Redirect, _) => Redirecting'.toEl(loggedInUser)
  | (AgentIndex, _) => AgentIndex'.toEl(loggedInUser)
  | (EntityShow(id), _) => EntityShow'.toEl({id: id}, loggedInUser)
  | (EntityIndex, _) => EntityIndex'.toEl(loggedInUser)
  | (Profile, _) => Profile'.toEl(loggedInUser)
  | (Preferences, _) => Preferences'.toEl(loggedInUser)
  | (Subscribe, _) => Preferences'.toEl(loggedInUser)
  | (Unsubscribe, _) => Preferences'.toEl(loggedInUser)
  | (ChannelNew, _) => ChannelNew'.toEl(loggedInUser)
  | (MeasurableEdit(id), _) => MeasurableEdit'.toEl({id: id}, loggedInUser)
  | (BotCreate, _) => BotCreate'.toEl(loggedInUser)
  | (BotEdit(botId), _) => BotEdit'.toEl({id: botId}, loggedInUser)
  | (Privacy, _) => <StaticPageInCard markdown=StaticMarkdown.privacyPolicy />
  | (Terms, _) =>
    <StaticPageInCard markdown=StaticMarkdown.termsAndConditions />
  | (ChannelIndex, Some(loggedInUser)) =>
    <FillWithSidebar channelId=None loggedInUser>
      <ChannelIndex.Jsx2 pageParams={id: "sd"} />
    </FillWithSidebar>
  | (Login, _) => <Login />
  | (_, _) => <Home />
  };
};

let component = "Layout" |> ReasonReact.statelessComponent;

let make = (~route: Route.t, _children) => {
  ...component,
  render: _ => {
    let innerComponentFn = toRoutePage(route);

    <Providers.AppContext.Consumer>
      ...{context => {
        let serverJwt = ServerJwt.make_from_storage();
        let auth0tokens = Auth0Tokens.make_from_storage();
        let authToken = context.authToken;

        switch (serverJwt, authToken, auth0tokens) {
        | (Some(_), _, _) => UserGet.inner(innerComponentFn)
        | (_, None, None) => innerComponentFn(Me.WithoutTokens)
        | (_, _, _) =>
          Authentication.component(
            auth0tokens,
            authToken,
            UserGet.inner(innerComponentFn),
          )
        };
      }}
    </Providers.AppContext.Consumer>;
  },
};