/* TODO: Allow for cases where user is not logged in */
let makeWithPage =
    (
      agentPage: Context.Routing.AgentPage.t,
      loggedInUser: option(Client.Context.Primary.User.t),
    )
    : ReasonReact.reactElement => {
  let agentId = agentPage.agentId;
  switch (loggedInUser) {
  | Some(loggedInUser) =>
    let layout = Agent_Layout_C.makeWithEl(agentPage, loggedInUser);
    let toEl = fn => fn |> E.React.makeToEl(~key=agentId);
    switch (agentPage.subPage) {
    | AgentShow => AgentShow.make(~pageParams={id: agentId}, ~layout) |> toEl
    | AgentMeasurables =>
      AgentMeasurables.make(
        ~pageParams={id: agentId},
        ~loggedInUser,
        ~layout,
      )
      |> toEl
    | AgentBots => AgentBots.make(~pageParams={id: agentId}, ~layout) |> toEl
    };
  | None => <Home />
  };
};