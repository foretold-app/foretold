open Utils;

module Component = {
  let name = (page: Routing.AgentPage.SubPage.t) =>
    switch (page) {
    | AgentMeasurements => "Predictions"
    | AgentMeasurables => "Created Questions"
    | AgentBots => "Bots"
    | AgentCommunities => "Communities"
    | AgentUpdates => "Activity"
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

  let tabs = (page: Routing.AgentPage.t, agent: AgenGet.agent) => {
    let agentId = page.agentId;
    let subPage = page.subPage;

    let agentType =
      switch (agent) {
      | {bot: Some(_)} => `BOT
      | _ => `USER
      };

    agentType == `USER
      ? <>
          {tab(agentId, subPage, AgentUpdates)}
          {tab(agentId, subPage, AgentCommunities)}
          {tab(agentId, subPage, AgentBots)}
        </>
      : <> {tab(agentId, subPage, AgentUpdates)} </>;
  };
};