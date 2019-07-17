open Rationale.Function.Infix;

module Wrapper = {
  type channelId = string;
  type loggedInUser = Primary.User.t;

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
};

module Renderer = {
  type channelId = string;
  type channelPage = Routing.ChannelPage.t;
  type loggedInUser = Primary.User.t;

  type t =
    | Channel(channelPage, loggedInUser)
    | NoChannel;

  let normal = SLayout.FullPage.makeWithEl;
  let channelLayout = (channelPage, loggedInUser) =>
    Channel_Layout_C.makeWithEl(channelPage, loggedInUser);

  let toComponent = t =>
    switch (t) {
    | Channel(channelPage, loggedInUser) =>
      channelLayout(channelPage, loggedInUser, None)
    | NoChannel => normal
    };
};

module LayoutWrapper = {
  let standardLayoutAndWrapper =
      (
        loggedInUser,
        make:
          (
            ~layout: 'a => ReasonReact.reactElement=?,
            ReasonReact.reactElement
          ) =>
          'b,
      ) =>
    make(~layout=Renderer.normal)
    |> Wrapper.noChannelSidebar(~key="", loggedInUser);
};

module LoggedInPage = {
  type pageParams = {id: string};

  let justPageParams = (make, pageParams: pageParams, loggedInUser) => {
    let makeWithUser = loggedInUser =>
      make(~pageParams)
      |> LayoutWrapper.standardLayoutAndWrapper(loggedInUser);

    loggedInUser |> E.O.fmap(makeWithUser) |> E.O.default(<Home />);
  };

  let justLoggedInUser = (make, loggedInUser) => {
    let makeWithUser = loggedInUser =>
      make(~loggedInUser)
      |> LayoutWrapper.standardLayoutAndWrapper(loggedInUser);

    loggedInUser |> E.O.fmap(makeWithUser) |> E.O.default(<Home />);
  };

  let pageParamsAndUser = (make, pageParams: pageParams, loggedInUser) => {
    let makeWithUser = loggedInUser =>
      make(~pageParams, ~loggedInUser)
      |> LayoutWrapper.standardLayoutAndWrapper(loggedInUser);

    loggedInUser |> E.O.fmap(makeWithUser) |> E.O.default(<Home />);
  };

  let noParams = (make, loggedInUser) => {
    let makeWithUser = loggedInUser =>
      make |> LayoutWrapper.standardLayoutAndWrapper(loggedInUser);

    loggedInUser |> E.O.fmap(makeWithUser) |> E.O.default(<Home />);
  };
};