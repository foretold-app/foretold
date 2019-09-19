let component = ReasonReact.statelessComponent("AgentScores");
let make = (~agentId, ~layout=SLayout.FullPage.makeWithEl, _children) => {
  ...component,
  render: _ => {
    let head =
      <div>
        <FC.Base.Div float=`left>
          <FC.PageCard.HeaderRow.Title>
            {"Scores" |> ReasonReact.string}
          </FC.PageCard.HeaderRow.Title>
        </FC.Base.Div>
      </div>;

    let body =
      <FC.PageCard.Body>
        <LeaderboardMembers agentId={Some(agentId)} />
      </FC.PageCard.Body>;

    SLayout.LayoutConfig.make(~head, ~body) |> layout;
  },
};