open Routing;
open Pages;

// @todo: fix this duplicated ligic(#rederectings)
let defaultPage = (loggedInUser: Types.user) =>
  loggedInUser.agent
  |> E.O.bind(_, Primary.Agent.firstChannel)
  |> E.O.fmap((channel: Types.channel) => {
       Routing.Url.push(ChannelShow(channel.id));
       ReasonReact.null;
     })
  |> {
    Primary.Channel.globalLink()
    |> Routing.Url.toString
    |> ReasonReact.Router.push;
    E.O.default(<Home />);
  };

type pageParams = {id: string};

let component = ReasonReact.statelessComponent("Navigator");

let make = (~route: Route.t, ~loggedInUser: option(Types.user), _children) => {
  ...component,
  render: _ => {
    switch (route, loggedInUser) {
    | (Home, Some(loggedInUser)) => defaultPage(loggedInUser)
    | (Channel(channel), _) =>
      Channel_Layout.makeWithPage(channel, loggedInUser)
    | (Agent(agentPage), _) =>
      Agent_Layout.makeWithPage(agentPage, loggedInUser)
    | (AgentIndex, _) => AgentIndex'.toEl(loggedInUser)
    | (EntityShow(id), _) => EntityShow'.toEl({id: id}, loggedInUser)
    | (EntityIndex, _) => EntityIndex'.toEl(loggedInUser)
    | (Profile, _) => Profile'.toEl(loggedInUser)
    | (Preferences, _) => Preferences'.toEl(loggedInUser)
    | (Subscribe, _) => Preferences'.toEl(loggedInUser)
    | (Unsubscribe, _) => Preferences'.toEl(loggedInUser)
    | (ChannelNew, _) => ChannelNew'.toEl(loggedInUser)
    | (MeasurableEdit(id), _) =>
      MeasurableEdit'.toEl({id: id}, loggedInUser)
    | (BotCreate, _) => BotCreate'.toEl(loggedInUser)
    | (BotEdit(botId), _) => BotEdit'.toEl({id: botId}, loggedInUser)
    | (Privacy, _) =>
      <StaticPageInCard markdown=StaticMarkdown.privacyPolicy />
    | (Terms, _) =>
      <StaticPageInCard markdown=StaticMarkdown.termsAndConditions />
    | (ChannelIndex, Some(loggedInUser)) =>
      <FillWithSidebar loggedInUser> <ChannelIndex.Jsx2 /> </FillWithSidebar>
    | (Login, _) => <Login />
    | (_, _) => <Home />
    };
  },
};