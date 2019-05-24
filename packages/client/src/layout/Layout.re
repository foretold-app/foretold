open Context.Routing;
open Foretold__GraphQL;
open Rationale.Function.Infix;
open Pages;

let defaultPage = (loggedInUser: option(Context.Primary.User.t)) =>
  loggedInUser
  |> E.O.bind(_, loggedInUser =>
       loggedInUser.agent
       |> E.O.bind(_, Context.Primary.Agent.firstChannel)
       |> E.O.fmap((channel: Context.Primary.Types.channel) => {
            Context.Routing.Url.push(ChannelShow(channel.id));
            <Home />;
          })
     )
  |> E.O.default(<Home />);

let meToUser = (me: Context.Me.me) =>
  switch (me) {
  | WithTokensAndUserData({userData}) => Some(userData)
  | _ => None
  };

let toRoutePage = (route: Route.t, me: Context.Me.me) => {
  let loggedInUser = meToUser(me);
  switch (route) {
  | Home => defaultPage(loggedInUser)
  | Login => <Login />
  | Channel(channel) => Channel_Layout.makeWithPage(channel, loggedInUser)
  | Redirect => Auth0Redirect'.toEl(loggedInUser)
  | AgentMeasurables(id) => AgentMeasurables'.toEl({id: id}, loggedInUser)
  | BotCreate => BotCreate'.toEl(loggedInUser)
  | AgentIndex => AgentIndex'.toEl(loggedInUser)
  | EntityShow(id) => EntityShow'.toEl({id: id}, loggedInUser)
  | EntityIndex => EntityIndex'.toEl(loggedInUser)
  | Profile => Profile'.toEl(loggedInUser)
  | AgentShow(id) => AgentShow'.toEl({id: id}, loggedInUser)
  | AgentBots(id) => AgentBots'.toEl({id: id}, loggedInUser)
  | ChannelIndex => ChannelIndex'.toEl(loggedInUser)
  | ChannelNew => ChannelNew'.toEl(loggedInUser)
  | MeasurableEdit(id) => MeasurableEdit'.toEl({id: id}, loggedInUser)
  | _ => <Home />
  };
};

let make = (route: Route.t) =>
  toRoutePage(route) |> Queries.User.withLoggedInUserQuery;