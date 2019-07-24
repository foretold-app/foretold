/* TODO: Allow for cases where user is not logged in */
let makeWithPage =
    (agentPage: Routing.AgentPage.t, loggedInUser: option(Primary.User.t))
    : ReasonReact.reactElement => {
  let agentId = agentPage.agentId;

  switch (loggedInUser) {
  | Some(loggedInUser) =>
    let layout = Agent_Layout_C.makeWithEl(agentPage, loggedInUser);
    let toEl = fn => fn |> E.React.makeToEl(~key=agentId);
    switch (agentPage.subPage) {
    | AgentMeasurements =>
      AgentMeasurements.make(
        ~pageParams={id: agentId},
        ~loggedInUser,
        ~layout,
      )
      |> toEl
    | AgentMeasurables =>
      AgentMeasurables.make(
        ~pageParams={id: agentId},
        ~loggedInUser,
        ~layout,
      )
      |> toEl
    | AgentBots => AgentBots.make(~pageParams={id: agentId}, ~layout) |> toEl
    | AgentCommunities =>
      AgentBots.make(~pageParams={id: agentId}, ~layout) |> toEl
    };

  | None => <Home />
  };
};