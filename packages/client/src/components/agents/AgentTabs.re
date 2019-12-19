open Utils;

let name = (page: Routing.AgentPage.SubPage.t) =>
  switch (page) {
  | AgentMeasurables => "Created Questions"
  | AgentBots => "Bots"
  | AgentCommunities => "Communities"
  | AgentUpdates => "Activity"
  | AgentScores => "Scores"
  };

let tab = (agentId, currentPage, selectedPage) => {
  let isActive = currentPage == selectedPage;
  <FC.Tab
    isActive
    onClick={LinkType.onClick(
      Internal(Agent({agentId, subPage: selectedPage})),
    )}>
    {name(selectedPage) |> ste}
  </FC.Tab>;
};

let tabs = (page: Routing.AgentPage.t, agent: Types.agent) => {
  let agentId = page.agentId;
  let subPage = page.subPage;

  switch (agent.agentType) {
  | Some(User(_)) =>
    <>
      {tab(agentId, subPage, AgentUpdates)}
      {tab(agentId, subPage, AgentCommunities)}
      {tab(agentId, subPage, AgentBots)}
      {tab(agentId, subPage, AgentScores)}
    </>
  | _ => <> {tab(agentId, subPage, AgentUpdates)} </>
  };
};