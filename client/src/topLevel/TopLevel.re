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

    (
      (loggedInUser: GetUser.t) =>
        <div>
          {
            switch (route) {
            | Home => <MeasurableIndex channel="general" />
            | AgentMeasurables(id) => <MeMeasurables id />
            | AgentIndex => <AgentIndex />
            | NotFound => <MeasurableIndex channel="general" />
            | ItemShow(id) => <ItemShow id />
            | Redirect => <Redirect />
            | Profile(_auth0Id) => <Profile loggedInUser />
            | AgentShow(id) => <AgentShow id />
            | Channel(channel) => <MeasurableIndex channel />
            | MeasurableNew(channel) => <MeasurableNew channel />
            | MeasurableShow(_, id) => <MeasurableShow id loggedInUser />
            | MeasurableEdit(id) => <MeasurableEdit id />
            | Series(channel, id) => <SeriesShow channel id loggedInUser />
            }
          }
        </div>
        |> FillWithSidebar.make(~channel, ~loggedInUser)
        |> ReasonReact.element
    )
    |> GetUser.withLoggedInUserQuery;
  },
};