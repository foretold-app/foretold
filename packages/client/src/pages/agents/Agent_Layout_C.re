let component = ReasonReact.statelessComponent("AgentLayoutPage");
let make =
    (
      agentPage: Routing.AgentPage.t,
      loggedInUser: Types.user,
      {head, body}: SLayout.LayoutConfig.t,
    ) => {
  ...component,
  render: _ => {
    <FC.PageCard>
      <FC.PageCard.HeaderRow> head </FC.PageCard.HeaderRow>
      body
    </FC.PageCard>;
  },
};

let makeWithEl =
    (agentPage: Routing.AgentPage.t, loggedInUser, t: SLayout.LayoutConfig.t) =>
  make(agentPage, loggedInUser, t) |> E.React.el;