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
        <>
          <ForetoldComponents.GroupHeader>
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
          </ForetoldComponents.GroupHeader>
          <ForetoldComponents.GroupHeader.SubHeader>
            <AgentTabs.Tabs agentPage agent />
          </ForetoldComponents.GroupHeader.SubHeader>
        </>;
      | _ => <Null />
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
     | AgentUpdates => <FeedItemsIndex agentId={Some(agentId)} />
     | AgentScores => <AgentScores agentId={Some(agentId)} />
     | _ => <NotFoundPage />
     }}
  </FillWithSidebar>;
};