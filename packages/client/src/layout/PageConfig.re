open Rationale.Function.Infix;

module Wrapper = {
  type channelId = string;
  type loggedInUser = Types.user;

  type t =
    | Channel(channelId)
    | NoChannel(loggedInUser);

  let channelWrapper = (channelId, fn) =>
    fn |> E.React.makeToEl(~key=channelId);

  let noChannelSidebar = (loggedInUser, ~key="") =>
    E.React.makeToEl
    ||> E.React.withParent(
          ~key,
          FillWithSidebar.make(~channelId=None, ~loggedInUser),
        );
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
    make(~layout=SLayout.FullPage.makeWithEl)
    |> Wrapper.noChannelSidebar(~key="", loggedInUser);
};

// @todo: do not use these wrappers it is not readable
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