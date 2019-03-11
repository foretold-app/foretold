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
      | Home => Some("general")
      | _ => None
      };

    (
      userQuery =>
        <div>
          {
            switch (route) {
            | Home => <MeasurableIndex channel="general" />
            | AgentMeasurables(id) => <MeMeasurables id />
            | AgentIndex => <AgentIndex />
            | NotFound => <MeasurableIndex channel="general" />
            | ItemShow(id) => <ItemShow id />
            | Redirect => <Redirect />
            | Profile(auth0Id) => <Profile auth0Id />
            | AgentShow(id) => <AgentShow id />
            | Channel(channel) => <MeasurableIndex channel />
            | MeasurableNew(channel) => <MeasurableNew channel />
            | MeasurableShow(_, id) => <MeasurableShow id />
            | MeasurableEdit(id) => <MeasurableEdit id />
            }
          }
        </div>
        |> FillWithSidebar.make(~channel, ~userQuery)
        |> ReasonReact.element
    )
    |> SharedQueries.withLoggedInUserQuery;
  },
};