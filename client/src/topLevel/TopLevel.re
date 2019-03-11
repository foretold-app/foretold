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
            | AgentMeasurables(id) => <AgentMeasurables id loggedInUser />
            | AgentIndex => <AgentIndex />
            | ItemShow(id) => <ItemShow id />
            | Redirect => <Redirect />
            | Profile => <Profile loggedInUser />
            | AgentShow(id) => <AgentShow id />
            | Channel(channel) => <MeasurableIndex channel loggedInUser />
            | MeasurableNew(channel) => <MeasurableNew channel />
            | MeasurableShow(_, id) => <MeasurableShow id loggedInUser />
            | MeasurableEdit(id) => <MeasurableEdit id />
            | Series(channel, id) => <SeriesShow channel id loggedInUser />
            | Home
            | NotFound => <MeasurableIndex channel="general" loggedInUser />
            }
          }
        </div>
        |> FillWithSidebar.make(~channel, ~loggedInUser)
        |> ReasonReact.element
    )
    |> GetUser.withLoggedInUserQuery;
  },
};