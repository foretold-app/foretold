open Context.Routing;
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

  let noChannelSidebar2 = (loggedInUser, ~key="") =>
    E.React.withParent(
      ~key,
      Layout__Component__FillWithSidebar.make(~channelId=None, ~loggedInUser),
    );
};

module Renderer = {
  type channelId = string;
  type channelPage = Context.Routing.Route.channelPage;
  type loggedInUser = Context.Primary.User.t;

  type t =
    | Channel(channelPage, loggedInUser)
    | NoChannel;

  let normal = SLayout.FullPage.makeWithEl;
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
    | NoChannel => normal
    };
};

let showHomeIfNoUser =
    (
      fn: Context.Primary.User.t => ReasonReact.reactElement,
      loggedInUser: option(Context.Primary.User.t),
    ) =>
  loggedInUser |> E.O.fmap(fn) |> E.O.default(<Home />);

module LayoutWrapper = {

let standard =
    (
      loggedInUser,
      make:
        (
          ~layout: 'a => ReasonReact.reactElement=?,
          ReasonReact.reactElement
        ) =>
        'b
    ) =>
  make(~layout=Renderer.normal)
  |> Wrapper.noChannelSidebar(~key="", loggedInUser);

}

let justPageParams = (make, pageParams, loggedInUser) =>{
    let makeWithUser = loggedInUser => (make(~pageParams) |> LayoutWrapper.standard(loggedInUser));
    showHomeIfNoUser(makeWithUser,loggedInUser);
  }

let justLoggedInUser = (make, loggedInUser) =>{
    let makeWithUser = loggedInUser => (make(~loggedInUser) |> LayoutWrapper.standard(loggedInUser));
    showHomeIfNoUser(makeWithUser,loggedInUser);
  }

let pageParamsAndUser = (make, pageParams, loggedInUser) =>{
    let makeWithUser = loggedInUser => (make(~pageParams, ~loggedInUser) |> LayoutWrapper.standard(loggedInUser));
    showHomeIfNoUser(makeWithUser,loggedInUser);
  }

let noParams = (make, loggedInUser) =>{
    let makeWithUser = loggedInUser => (make |> LayoutWrapper.standard(loggedInUser));
    showHomeIfNoUser(makeWithUser,loggedInUser);
  }

module EntityShow' = {
  let toEl = EntityShow.make |> justPageParams
};

module AgentMeasurables' = {
  let toEl = AgentMeasurables.make |> pageParamsAndUser
};

module BotCreate' = {
  let toEl = BotCreate.make |> noParams
};

module AgentIndex' = {
  let toEl = AgentIndex.make |> noParams
};

module AgentShow' = {
  let toEl = AgentShow.make |> justPageParams
};

module Profile' = {
  let toEl = Profile.make |> justLoggedInUser
};

module AgentBots' = {
  let toEl = AgentBots.make |> justPageParams
};

module ChannelIndex' = {
  let toEl = ChannelIndex.make |> justLoggedInUser
};

module ChannelNew' = {
  let toEl = ChannelNew.make |> noParams
};

module MeasurableEdit' = {
  let toEl = MeasurableEdit.make |> justPageParams
};

module EntityIndex' = {
  let toEl = EntityIndex.make |> noParams
};
