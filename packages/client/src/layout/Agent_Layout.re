/* TODO: Allow for cases where user is not logged in */
let makeWithPage =
    (agentPage: Routing.AgentPage.t, loggedInUser: option(Types.user))
    : ReasonReact.reactElement => {
  let agentId = agentPage.agentId;

  switch (loggedInUser) {
  | Some(loggedInUser) =>
    let layout = Agent_Layout_C.makeWithEl(agentPage, loggedInUser);

    switch (agentPage.subPage) {
    | AgentMeasurements =>
      <AgentMeasurements pageParams={id: agentId} loggedInUser layout />
    | AgentMeasurables =>
      <AgentMeasurables pageParams={id: agentId} loggedInUser layout />
    | AgentBots => <AgentBots pageParams={id: agentId} layout />
    | AgentCommunities => <AgentCommunities agentId layout />
    | AgentUpdates => <FeedItems agentId layout />
    };

  | None => <Home />
  };
};