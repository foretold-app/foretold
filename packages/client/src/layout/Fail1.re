/* open Context.Routing;
   open Foretold__GraphQL;
   open Rationale.Function.Infix;
   module Wrapper = {
     type channelId = string;
     type loggedInUser = Client.Context.Primary.User.t;

     type t =
       | Channel(channelId)
       | NoChannel(loggedInUser);

     let channelWrapper = (channelId, fn) =>
       fn |> E.React.makeToEl(~key=channelId);

     let noChannelSidebar = (loggedInUser, ~key="") =>
       E.React.makeToEl
       ||> E.React.withParent(
             ~key,
             Layout__Component__FillWithSidebar.make(
               ~channelId=None,
               ~loggedInUser,
             ),
           );

     let fromRoute = (route: Context.Routing.Route.t, loggedInUser) =>
       switch (route) {
       | Channel({channelId, _}) => Channel(channelId)
       | AgentMeasurables(_)
       | BotCreate
       | AgentIndex
       | EntityShow(_)
       | Redirect
       | EntityIndex
       | Profile
       | AgentShow(_)
       | AgentBots(_)
       | ChannelIndex
       | MeasurableEdit(_)
       | _ => NoChannel(loggedInUser)
       };

     let toComponent = t =>
       switch (t) {
       | Channel(channelId) => channelWrapper(channelId)
       | NoChannel(loggedInUser) => noChannelSidebar(loggedInUser, ~key="")
       };
   };

   module Layout = {
     type channelId = string;
     type channelPage = Context.Routing.Route.channelPage;
     type loggedInUser = Context.Primary.User.t;

     type t =
       | Channel(channelPage, loggedInUser)
       | NoChannel;

     let noChannelLayout = SLayout.FullPage.makeWithEl;
     let channelLayout = (channelPage, loggedInUser) =>
       Channel_Layout_C.makeWithEl(channelPage, loggedInUser);

     let fromRoute = (route: Context.Routing.Route.t, loggedInUser) =>
       switch (route) {
       | Channel(channelPage) => Channel(channelPage, loggedInUser)
       | AgentMeasurables(_)
       | BotCreate
       | AgentIndex
       | EntityShow(_)
       | Redirect
       | EntityIndex
       | Profile
       | AgentShow(_)
       | AgentBots(_)
       | ChannelIndex
       | MeasurableEdit(_)
       | _ => NoChannel
       };

     let toComponent = t =>
       switch (t) {
       | Channel(channelPage, loggedInUser) =>
         channelLayout(channelPage, loggedInUser)
       | NoChannel => noChannelLayout
       };
   };

   module type Config = {
     type customParams;
     type layoutConfig;
     let loggedOutPage: ReasonReact.reactElement;
     let layout: layoutConfig => ReasonReact.reactElement;
     let wrapper:
       (
         Client.Context.Primary.User.t,
         ReasonReact.reactElement => ReasonReact.component('a, 'b, 'c)
       ) =>
       ReasonReact.reactElement;
     let make:
       (customParams, ~layout: layoutConfig => ReasonReact.reactElement=?, 'a) =>
       ReasonReact.componentSpec(
         ReasonReact.stateless,
         ReasonReact.stateless,
         ReasonReact.noRetainedProps,
         ReasonReact.noRetainedProps,
         ReasonReact.actionless,
       );
   };

   module AgentShoww: Config = {
     type customParams = string;
     type layoutConfig = Client.SLayout.LayoutConfig.t;
     let layout = Layout.noChannelLayout;

     let make = (id: customParams) => AgentShow.make(~id);

     let wrapper = loggedInUser =>
       Wrapper.noChannelSidebar(loggedInUser, ~key="");

     let loggedOutPage = <Home />;
   };

   module Generator = (Config: Config) => {
     module Config = Config;

     let generate =
         (
           loggedInUser: option(Context.Primary.User.t),
           params: Config.customParams,
         ) =>
       loggedInUser
       |> E.O.fmap(loggedInUser => {
            let layout = Config.layout;
            Config.make(params, ~layout) |> Config.wrapper(loggedInUser);
          })
       |> E.O.default(Config.loggedOutPage);
   };

   module Barr = Generator(AgentShoww); */