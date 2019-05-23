open Context.Routing;
open Foretold__GraphQL;
open Rationale.Function.Infix;
open Pages;

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
};

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

  let noChannelSidebar2 = (loggedInUser, ~key="") =>
    E.React.withParent(
      ~key,
      Layout__Component__FillWithSidebar.make(~channelId=None, ~loggedInUser),
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
};

let showHomeIfNoUser =
    (
      loggedInUser: option(Context.Primary.User.t),
      fn: Context.Primary.User.t => ReasonReact.reactElement,
    ) =>
  loggedInUser |> E.O.fmap(fn) |> E.O.default(<Home />);

let toRoutePage = (route: Route.t, me: Context.Me.me) =>
  switch (me) {
  | WithTokensAndUserData({userData}) =>
    let loggedInUser = userData;

    let sidebar =
      Layout__Component__FillWithSidebar.make(~channelId=None, ~loggedInUser);

    let inApp = (~key="") =>
      E.React.makeToEl ||> E.React.withParent(~key, sidebar);

    let layout = SLayout.FullPage.makeWithEl;

    /* let sLU; */
    switch (route) {
    | Redirect => Auth0Redirect.make(~me, ~layout) |> inApp
    | Channel(channel) => Channel_Layout.makeWithPage(channel, loggedInUser)
    | AgentMeasurables(id) =>
      AgentMeasurables'.toEl({id: id}, Some(loggedInUser))
    | BotCreate => BotCreate'.toEl(Some(loggedInUser))
    | AgentIndex => AgentIndex'.toEl(Some(loggedInUser))
    | EntityShow(id) => EntityShow'.toEl({id: id}, Some(loggedInUser))
    | EntityIndex => EntityIndex'.toEl(Some(loggedInUser))
    | Profile => Profile'.toEl(Some(loggedInUser))
    | AgentShow(id) => AgentShow'.toEl({id: id}, Some(loggedInUser))
    | AgentBots(id) => AgentBots'.toEl({id: id}, Some(loggedInUser))
    | ChannelIndex => ChannelIndex'.toEl(Some(loggedInUser))
    | ChannelNew => ChannelNew'.toEl(Some(loggedInUser))
    | MeasurableEdit(id) =>
      MeasurableEdit'.toEl({id: id}, Some(loggedInUser))
    | _ => defaultPage(loggedInUser, layout)
    };
  | _ =>
    switch (route) {
    | Home => <Home />
    | Login => <Login />
    | Redirect => <Auth0Redirect me />
    | _ => <Home />
    }
  };

let make = (route: Route.t) =>
  toRoutePage(route) |> Queries.User.withLoggedInUserQuery;