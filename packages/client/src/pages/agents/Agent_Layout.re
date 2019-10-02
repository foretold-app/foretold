open Style.Grid;

module Top = {
  let component = ReasonReact.statelessComponent("Top");
  let make =
      (~agentPage: Routing.AgentPage.t, ~loggedInUser: Types.user, _children) => {
    ...component,
    render: _ => {
      let agentId = agentPage.agentId;

      AgentGet.component(
        ~id=agentId,
        ({agent}) => {
          let name =
            switch (agent) {
            | {bot: Some(r)} => r.name
            | {user: Some(r)} => r.name
            | _ => ""
            };

          let description =
            (
              switch (agent) {
              | {bot: Some(r)} => r.description
              | {user: Some(r)} => r.description
              | _ => Some("")
              }
            )
            |> E.O.default("");

          let score =
            switch (agent) {
            | {user: Some(r)} =>
              r.score == None
                ? "(none)" : r.score |> E.O.default(0.) |> string_of_float
            | _ => "(none)"
            };

          let secondLevel = AgentTabs.tabs(agentPage, agent);

          <>
            <FC.GroupHeader>
              <div> {name |> Utils.ste} </div>
              <Div styles=[SLayout.Styles.descriptionText]>
                {"Percentage Question Brier Score: " |> Utils.ste}
                {score |> Utils.ste}
              </Div>
              <Div styles=[SLayout.Styles.descriptionText]>
                {description |> Utils.ste}
              </Div>
            </FC.GroupHeader>
            <FC.GroupHeader.SubHeader> secondLevel </FC.GroupHeader.SubHeader>
          </>;
        },
      );
    },
  };
};

module Agent_Layout_C = {
  let component = ReasonReact.statelessComponent("AgentLayoutPage");
  let make = ({head, body}: SLayout.LayoutConfig.t) => {
    ...component,
    render: _ => {
      <SLayout head> body </SLayout>;
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
        <Top agentPage loggedInUser />
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