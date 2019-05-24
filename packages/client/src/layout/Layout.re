open Context.Routing;
open Foretold__GraphQL;
open Rationale.Function.Infix;
open Pages;
/*
 let defaultPage = (loggedInUser: Context.Primary.User.t, layout) => {
   let firstUserChannel =
     loggedInUser.agent
     |> E.O.bind(_, (a: Context.Primary.Agent.t) => a.channelMemberships)
     |> E.A.O.defaultEmpty
     |> E.A.get(_, 0)
     |> E.O.bind(_, (r: Context.Primary.Types.channelMembership) => r.channel);

   let sidebar =
     Layout__Component__FillWithSidebar.make(
       ~channelId=
         firstUserChannel
         |> E.O.fmap((c: Context.Primary.Types.channel) => c.id),
       ~loggedInUser,
     );

   let inApp = (~key="") =>
     E.React.makeToEl ||> E.React.withParent(~key, sidebar);

   /* This should always be Some */
   switch (firstUserChannel) {
   | Some({id: channelIdSome}) =>
     Channel_Layout.makeWithPage(
       {channelId: channelIdSome, subPage: Measurables("")},
       loggedInUser,
     )
   | _ => ChannelIndex.make(~loggedInUser, ~layout) |> inApp
   };
 }; */

let meToUser = (me: Context.Me.me) =>
  switch (me) {
  | WithTokensAndUserData({userData}) => Some(userData)
  | _ => None
  };

let toRoutePage = (route: Route.t, me: Context.Me.me) => {
  let loggedInUser = meToUser(me);
  switch (route) {
  | Home => <Home />
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