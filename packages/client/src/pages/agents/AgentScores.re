open Style.Grid;

let component = ReasonReact.statelessComponent("AgentScores");
let make = (~agentId, ~layout=SLayout.FullPage.makeWithEl, _children) => {
  ...component,
  render: _ => {
    let head = (~channelId, ~paginationPage, ()) =>
      <div>
        <FC.Base.Div float=`left>
          <FC.PageCard.HeaderRow.Title>
            {"Scores" |> ReasonReact.string}
          </FC.PageCard.HeaderRow.Title>
        </FC.Base.Div>
        <Div>
          <Div
            float=`right
            styles=[Css.style([FC.PageCard.HeaderRow.Styles.itemTopPadding])]>
            paginationPage
          </Div>
        </Div>
      </div>;

    <LeaderboardMembers agentId={Some(agentId)} head />;

    let body =
      <FC.PageCard.Body>
        <LeaderboardMembers agentId={Some(agentId)} head />
      </FC.PageCard.Body>;

    let head =
      <div>
        <FC.Base.Div float=`left>
          <FC.PageCard.HeaderRow.Title>
            {"Communities" |> ReasonReact.string}
          </FC.PageCard.HeaderRow.Title>
        </FC.Base.Div>
      </div>;

    SLayout.LayoutConfig.make(~head, ~body) |> layout;
  },
};