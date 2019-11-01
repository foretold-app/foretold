[@bs.config {jsx: 3}];

open Style.Grid;

module Top = {
  [@react.component]
  let make = (~agentPage: Routing.AgentPage.t) => {
    let agentId = agentPage.agentId;

    AgentGet.component(~id=agentId, agent =>
      switch (agent) {
      | Success(Some(agent)) =>
        let description =
          (
            switch (agent.agentType) {
            | Some(Bot({description})) => description
            | Some(User({description})) => description
            | _ => None
            }
          )
          |> E.O.default("");

        let secondLevel = AgentTabs.tabs(agentPage, agent);

        <>
          <FC.GroupHeader>
            <Div flexDirection=`column>
              <Div
                flex={`num(1.)}
                styles=[
                  Css.style([
                    Css.fontSize(`em(1.2)),
                    Css.marginBottom(`em(0.1)),
                  ]),
                ]>
                <AgentLink agent />
              </Div>
              <Div flex={`num(1.)} styles=[SLayout.Styles.descriptionText]>
                {description |> Utils.ste}
              </Div>
            </Div>
          </FC.GroupHeader>
          <FC.GroupHeader.SubHeader> secondLevel </FC.GroupHeader.SubHeader>
        </>;
      | _ => ReasonReact.null
      }
    );
  };
};

[@react.component]
let make = (~agentPage: Routing.AgentPage.t) => {
  let agentId = agentPage.agentId;

  <FillWithSidebar>
    <Top agentPage />
    {switch (agentPage.subPage) {
     | AgentMeasurables => <AgentMeasurables pageParams={id: agentId} />
     | AgentBots => <AgentBots pageParams={id: agentId} />
     | AgentCommunities => <AgentCommunities agentId />
     | AgentUpdates => <FeedItems agentId={Some(agentId)} />
     | AgentScores => <AgentScores agentId={Some(agentId)} />
     }}
  </FillWithSidebar>;
};