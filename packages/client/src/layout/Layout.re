open Routing;
open Pages;

let defaultPage = (loggedInUser: option(Primary.User.t)) =>
  loggedInUser
  |> E.O.bind(_, loggedInUser =>
       loggedInUser.agent
       |> E.O.bind(_, Primary.Agent.firstChannel)
       |> E.O.fmap((channel: Types.channel) => {
            Routing.Url.push(ChannelShow(channel.id));
            <Home />;
          })
     )
  |> E.O.default(ChannelIndex'.toEl(loggedInUser));

let meToUser = (me: Me.t) =>
  switch (me) {
  | WithTokensAndUserData({userData}) => Some(userData)
  | _ => None
  };

let toRoutePage = (route: Route.t, me: Me.t) => {
  let loggedInUser = meToUser(me);

  switch (route) {
  | Home => defaultPage(loggedInUser)
  | Login => <Login />
  | Privacy => <StaticPageInCard markdown=StaticMarkdown.privacyPolicy />
  | Terms => <StaticPageInCard markdown=StaticMarkdown.termsAndConditions />
  | Channel(channel) => Channel_Layout.makeWithPage(channel, loggedInUser)
  | Agent(agentPage) => Agent_Layout.makeWithPage(agentPage, loggedInUser)
  | Redirect => Redirecting'.toEl(loggedInUser)
  | AgentIndex => AgentIndex'.toEl(loggedInUser)
  | EntityShow(id) => EntityShow'.toEl({id: id}, loggedInUser)
  | EntityIndex => EntityIndex'.toEl(loggedInUser)
  | Profile => Profile'.toEl(loggedInUser)
  | Preferences => Preferences'.toEl(loggedInUser)
  | Subscribe => Preferences'.toEl(loggedInUser)
  | Unsubscribe => Preferences'.toEl(loggedInUser)
  | ChannelIndex => ChannelIndex'.toEl(loggedInUser)
  | ChannelNew => ChannelNew'.toEl(loggedInUser)
  | MeasurableEdit(id) => MeasurableEdit'.toEl({id: id}, loggedInUser)
  | BotCreate => BotCreate'.toEl(loggedInUser)
  | BotEdit(botId) => BotEdit'.toEl({id: botId}, loggedInUser)
  | _ => <Home />
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
        | (Some(_), _, _) => GetUser.inner(innerComponentFn)
        | (_, None, None) => innerComponentFn(Me.WithoutTokens)
        | (_, _, _) =>
          Authentication.component(
            auth0tokens,
            authToken,
            GetUser.inner(innerComponentFn),
          )
        };
      }}
    </Providers.AppContext.Consumer>;
  },
};