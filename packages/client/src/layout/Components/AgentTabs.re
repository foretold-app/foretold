/* TODO: Finish me */
open Utils;

module Component = {
  let name = (page: Context.Routing.AgentPage.SubPage.t) =>
    switch (page) {
    | AgentShow => "Predictions"
    | AgentMeasurables => "Created Questions"
    | AgentBots => "Bots"
    };

  let tab = (agentId, currentPage, selectedPage) => {
    let isActive = currentPage == selectedPage;
    <FC.Tab
      isActive
      onClick={
        C.Link.LinkType.onClick(
          Internal(Agent({agentId, subPage: selectedPage})),
        )
      }>
      {name(selectedPage) |> ste}
    </FC.Tab>;
  };

  let tabs = (page: Context.Routing.AgentPage.t) => {
    let agentId = page.agentId;
    let subPage = page.subPage;
    <>
      {tab(agentId, subPage, AgentShow)}
      {tab(agentId, subPage, AgentMeasurables)}
      {tab(agentId, subPage, AgentBots)}
    </>;
  };
};