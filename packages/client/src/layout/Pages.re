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
module EntityShow' = {
  let _loggedInMake = (id, loggedInUser) => {
    EntityShow.make(~id) |> LayoutWrapper.standard(loggedInUser);
  };
  let toEl = (id) =>
  id |> _loggedInMake |> showHomeIfNoUser;
};

module AgentShow' = {
  let _loggedInMake = (pageParams, loggedInUser) => {
    AgentShow.make(~pageParams) |> LayoutWrapper.standard(loggedInUser);
  };
  let toEl = (pageParams) =>
  pageParams |> _loggedInMake |> showHomeIfNoUser;
};

module Profile' = {
  let _loggedInMake = (loggedInUser) => {
    Profile.make(~loggedInUser) |> LayoutWrapper.standard(loggedInUser);
  };
  let toEl = _loggedInMake |> showHomeIfNoUser;
};

module AgentBots' = {
  let _loggedInMake = (pageParams, loggedInUser) => {
    AgentBots.make(~pageParams) |> LayoutWrapper.standard(loggedInUser);
  };
  let toEl = pageParams => pageParams |> _loggedInMake |> showHomeIfNoUser;
};

module ChannelIndex' = {
  let _loggedInMake = (loggedInUser) => {
    ChannelIndex.make(~loggedInUser) |> LayoutWrapper.standard(loggedInUser);
  };
  let toEl = _loggedInMake |> showHomeIfNoUser;
};

module ChannelNew' = {
  let _loggedInMake = (loggedInUser) => {
    ChannelNew.make |> LayoutWrapper.standard(loggedInUser);
  };
  let toEl = _loggedInMake |> showHomeIfNoUser;
};

module MeasurableEdit' = {
  let _loggedInMake = (pageParams, loggedInUser) => {
    MeasurableEdit.make(~pageParams) |> LayoutWrapper.standard(loggedInUser);
  };
  let toEl = pageParams => pageParams |>_loggedInMake |> showHomeIfNoUser;
};

module EntityIndex' = {
  let _loggedInMake = (loggedInUser) => {
    EntityIndex.make |> LayoutWrapper.standard(loggedInUser);
  };
  let toEl = _loggedInMake |> showHomeIfNoUser;
};
