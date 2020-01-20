open Utils;

let name = (page: Routing.AgentPage.SubPage.t) =>
  switch (page) {
  | AgentMeasurables => "Created Questions"
  | AgentBots => "Bots"
  | AgentCommunities => "Communities"
  | AgentUpdates => "Activity"
  | AgentScores => "Scores"
  };

module Tab = {
  [@react.component]
  let make = (~agentId, ~subPage, ~selectedPage) => {
    let isActive = subPage == selectedPage;
    <ForetoldComponents.Tab
      isActive
      onClick={LinkType.onClick(
        Internal(Agent({agentId, subPage: selectedPage})),
      )}>
      {name(selectedPage) |> ste}
    </ForetoldComponents.Tab>;
  };
};

module Tabs = {
  [@react.component]
  let make = (~agentPage: Routing.AgentPage.t, ~agent: Types.agent) => {
    let agentId = agentPage.agentId;
    let subPage = agentPage.subPage;

    switch (agent.agentType) {
    | Some(User(_)) =>
      <>
        <Tab
          agentId
          subPage
          selectedPage=Routing.AgentPage.SubPage.AgentUpdates
        />
        <Tab
          agentId
          subPage
          selectedPage=Routing.AgentPage.SubPage.AgentCommunities
        />
        <Tab
          agentId
          subPage
          selectedPage=Routing.AgentPage.SubPage.AgentBots
        />
        <Tab
          agentId
          subPage
          selectedPage=Routing.AgentPage.SubPage.AgentScores
        />
      </>
    | _ =>
      <Tab
        agentId
        subPage
        selectedPage=Routing.AgentPage.SubPage.AgentUpdates
      />
    };
  };
};