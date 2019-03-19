open Routes;
open Foretold__GraphQL;

let defaultChannel = "general";
let component = ReasonReact.statelessComponent("TopLevel");
let make = (~route: route, _children) => {
  ...component,
  render: _ => {
    let channel =
      switch (route) {
      | ChannelShow(c) => Some(c)
      | MeasurableNew(c) => Some(c)
      | MeasurableShow(c, _) => Some(c)
      | Series(c, _) => Some(c)
      | Home => Some(defaultChannel)
      | _ => None
      };

    Queries.User.withLoggedInUserQuery
    |> E.F.apply((loggedInUser: Queries.User.t) =>
         switch (loggedInUser) {
         | Some(_userIsLoggedIn) =>
           let inApp = (~key="", e) =>
             FillWithSidebar.make(~channel, ~loggedInUser)
             |> E.React.makeToEl(~children=E.React.makeToEl(e), ~key);
           switch (route) {
           | AgentMeasurables(id) =>
             AgentMeasurables.make(~id, ~loggedInUser) |> inApp
           | AgentIndex => AgentIndex.make |> inApp
           | ItemShow(id) => ItemShow.make(~id) |> inApp
           | Redirect => Redirect.make |> inApp
           | Profile => Profile.make(~loggedInUser) |> inApp
           | AgentShow(id) => AgentShow.make(~id) |> inApp
           | ChannelShow(channel) =>
             MeasurableIndex.make(~channel, ~loggedInUser)
             |> inApp(~key=channel)
           | ChannelIndex => ChannelIndex.make |> inApp
           | ChannelNew => ChannelNew.make(~channel=defaultChannel) |> inApp
           | MeasurableNew(channel) => MeasurableNew.make(~channel) |> inApp
           | MeasurableShow(channel, id) =>
             MeasurableShow.make(~channel, ~id, ~loggedInUser) |> inApp
           | MeasurableEdit(id) => MeasurableEdit.make(~id) |> inApp
           | Series(channel, id) =>
             SeriesShow.make(~id, ~channel, ~loggedInUser) |> inApp
           | _ =>
             MeasurableIndex.make(~channel=defaultChannel, ~loggedInUser)
             |> inApp
           };
         | None =>
           switch (route) {
           | Home => <Home />
           | Login => <Login />
           | _ => <Home />
           }
         }
       );
  },
};