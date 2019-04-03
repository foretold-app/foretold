open Context.Routing;
open Foretold__GraphQL;
open Rationale.Function.Infix;

let defaultChannel = "4c5af8ad-1258-4b9c-8fa0-cc522708fc1c";

let tochannelId = (route: Route.t) =>
  switch (route) {
  | ChannelShow(c) => Some(c)
  | ChannelEdit(c) => Some(c)
  | MeasurableNew(c) => Some(c)
  | Series(c, _) => Some(c)
  | Home => Some(defaultChannel)
  | _ => None
  };

let defaultPage =
    (
      loggedInUser: Context.Primary.User.t,
      layout,
      inApp:
        (
          ~key: string=?,
          ReasonReact.reactElement => ReasonReact.component('a, 'b, 'c)
        ) =>
        ReasonReact.reactElement,
    ) => {
  let firstUserChannel =
    loggedInUser.agent
    |> E.O.fmap((a: Context.Primary.Agent.t) => a.channels)
    |> E.O.bind(_, (r: Js.Array.t(Context.Primary.Channel.t)) =>
         E.A.get(r, 0)
       );

  /* This should always be Some */
  switch (firstUserChannel) {
  | Some({id: channelId}) =>
    MeasurableIndex.make(~channelId, ~loggedInUser, ~layout, ~itemsPerPage=20)
    |> inApp(~key=channelId)
  | _ => <Home/>
  };
};

let toRoutePage = (route: Route.t, me: Context.Me.me) =>
  switch (me) {
  | WithTokensAndUserData({userData}) =>
    let loggedInUser = userData;
    let channelId = tochannelId(route);
    let sidebar =
      Layout__Component__FillWithSidebar.make(~channelId, ~loggedInUser);

    let inApp = (~key="") =>
      E.React.makeToEl ||> E.React.withParent(~key, sidebar);

    let layout = SLayout.FullPage.makeWithEl;

    switch (route) {
    | AgentMeasurables(id) =>
      AgentMeasurables.make(~id, ~loggedInUser, ~layout) |> inApp
    | AgentIndex => AgentIndex.make(~layout) |> inApp
    | EntityShow(id) => EntityShow.make(~id, ~layout) |> inApp
    | Redirect => Auth0Redirect.make(~me) |> inApp
    | Profile => Profile.make(~loggedInUser, ~layout) |> inApp
    | AgentShow(id) => AgentShow.make(~id, ~layout) |> inApp
    | ChannelShow(channelId) =>
      MeasurableIndex.make(
        ~channelId,
        ~loggedInUser,
        ~layout,
        ~itemsPerPage=20,
      )
      |> inApp(~key=channelId)
    | ChannelEdit(channelId) =>
      ChannelEdit.make(~channelId, ~layout) |> inApp
    | ChannelMembers(channelId) =>
      ChannelMembers.make(~channelId, ~loggedInUser,~layout) |> inApp
    | ChannelIndex => ChannelIndex.make(~loggedInUser, ~layout) |> inApp
    | ChannelNew => ChannelNew.make(~layout) |> inApp
    | MeasurableNew(channelId) =>
      MeasurableNew.make(~channelId, ~layout) |> inApp
    | MeasurableEdit(id) => MeasurableEdit.make(~id, ~layout) |> inApp
    | Series(channelId, id) =>
      SeriesShow.make(~id, ~channelId, ~loggedInUser, ~layout) |> inApp
    | _ => defaultPage(loggedInUser, layout, inApp)
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
  route |> toRoutePage |> Queries.User.withLoggedInUserQuery;