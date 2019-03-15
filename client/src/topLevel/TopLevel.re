open Routes;

/* <MeasurableIndex channel loggedInUser key=channel />
   |> FillWithSidebar.make(~channel=Some(channel), ~loggedInUser)
   |> ReasonReact.element */
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
      | Home => Some("general")
      | _ => None
      };

    (
      (loggedInUser: GetUser.t) =>
        switch (loggedInUser) {
        | Some(_userIsLoggedIn) =>
          let inApp = (~key="", e) =>
            e
            |> (r => [||] |> r)
            |> ReasonReact.element
            |> FillWithSidebar.make(~channel, ~loggedInUser)
            |> ReasonReact.element(~key);
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
          | ChannelNew => ChannelNew.make(~channel="general") |> inApp
          | MeasurableNew(channel) => MeasurableNew.make(~channel) |> inApp
          | MeasurableShow(channel, id) =>
            MeasurableShow.make(~channel, ~id, ~loggedInUser) |> inApp
          | MeasurableEdit(id) => MeasurableEdit.make(~id) |> inApp
          | Series(channel, id) =>
            SeriesShow.make(~id, ~channel, ~loggedInUser) |> inApp
          | _ =>
            MeasurableIndex.make(~channel="general", ~loggedInUser) |> inApp
          };
        | None =>
          switch (route) {
          | Home => <Home />
          | Login => <Login />
          | _ => <Home />
          }
        }
    )
    |> GetUser.withLoggedInUserQuery;
  },
};