open Style.Grid;

module Top = {
  let component = ReasonReact.statelessComponent("Top");
  let make = (~agentPage: Routing.AgentPage.t, _children) => {
    ...component,
    render: _ => {
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
                  <AgentLink.Jsx2 agent />
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
    },
  };
};

module Agent_Layout_C = {
  let component = ReasonReact.statelessComponent("AgentLayoutPage");
  let make = ({head, body, isFluid}: SLayout.LayoutConfig.t) => {
    ...component,
    render: _ => {
      <SLayout head isFluid> body </SLayout>;
    },
  };

  let makeWithEl = (t: SLayout.LayoutConfig.t) => make(t) |> E.React.el;
};

let component = ReasonReact.statelessComponent("AgentLayout");
let make =
    (
      ~agentPage: Routing.AgentPage.t,
      ~loggedInUser: option(Types.user),
      _children,
    ) => {
  ...component,
  render: _ => {
    let agentId = agentPage.agentId;

    switch (loggedInUser) {
    | Some(loggedInUser) =>
      let layout = Agent_Layout_C.makeWithEl;

      <FillWithSidebar loggedInUser>
        <Top agentPage />
        {switch (agentPage.subPage) {
         | AgentMeasurables =>
           <AgentMeasurables pageParams={id: agentId} loggedInUser layout />
         | AgentBots => <AgentBots pageParams={id: agentId} layout />
         | AgentCommunities => <AgentCommunities agentId layout />
         | AgentUpdates => <FeedItems agentId={Some(agentId)} layout />
         | AgentScores => <AgentScores agentId={Some(agentId)} />
         }}
      </FillWithSidebar>;

    | None => <Home />
    };
  },
};