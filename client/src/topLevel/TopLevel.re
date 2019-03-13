open Routes;

let component = ReasonReact.statelessComponent("TopLevel");
let make = (~route: route, _children) => {
  ...component,
  render: _ => {
    let channel =
      switch (route) {
      | Channel(c) => Some(c)
      | MeasurableNew(c) => Some(c)
      | MeasurableShow(c, _) => Some(c)
      | Series(c, _) => Some(c)
      | Home => Some("general")
      | _ => None
      };

    let isLoggedInPage =
      switch (route) {
      | AgentMeasurables(_) => true
      | AgentIndex => true
      | ItemShow(_) => true
      | Redirect => false
      | Profile => true
      | AgentShow(_) => true
      | Channel(_) => true
      | MeasurableNew(_) => true
      | MeasurableShow(_) => true
      | MeasurableEdit(_) => true
      | Series(_) => true
      | Home => false
      | NotFound => false
      };

    let fr = ReasonReact.fragment;

    (
      (loggedInUser: GetUser.t) => {
        let withSidebar = e =>
          e
          |> ReasonReact.element
          |> FillWithSidebar.make(~channel, ~loggedInUser)
          |> ReasonReact.element;

        switch (route) {
        | AgentMeasurables(id) =>
          AgentMeasurables.make(~id, ~loggedInUser, fr) |> withSidebar
        | AgentIndex => AgentIndex.make(fr) |> withSidebar
        | ItemShow(id) => ItemShow.make(~id, fr) |> withSidebar
        | Redirect => Redirect.make(fr) |> withSidebar
        | Profile => Profile.make(~loggedInUser, fr) |> withSidebar
        | AgentShow(id) => AgentShow.make(~id, fr) |> withSidebar
        | Channel(channel) =>
          MeasurableIndex.make(~channel, ~loggedInUser, fr) |> withSidebar
        | MeasurableNew(channel) =>
          MeasurableNew.make(~channel, fr) |> withSidebar
        | MeasurableShow(_, id) =>
          MeasurableShow.make(~id, ~loggedInUser, fr) |> withSidebar
        | MeasurableEdit(id) => MeasurableEdit.make(~id, fr) |> withSidebar
        | Series(channel, id) =>
          SeriesShow.make(~id, ~channel, ~loggedInUser, fr) |> withSidebar
        | Home => Home.make(fr) |> ReasonReact.element
        | NotFound =>
          MeasurableIndex.make(~channel="general", ~loggedInUser, fr)
          |> withSidebar
        };
      }
    )
    |> GetUser.withLoggedInUserQuery;
  },
};