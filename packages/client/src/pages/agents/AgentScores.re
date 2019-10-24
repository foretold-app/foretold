[@bs.config {jsx: 3}];

open Style.Grid;

let component = ReasonReact.statelessComponent("AgentScores");
let make = (~agentId, _children) => {
  ...component,
  render: _ => {
    let head = (~channelId, ~paginationPage, ()) =>
      <div>
        <FC.Base.Div.Jsx2 float=`left>
          <FC.PageCard.HeaderRow.Title>
            {"Scores" |> ReasonReact.string}
          </FC.PageCard.HeaderRow.Title>
        </FC.Base.Div.Jsx2>
        <Div>
          <Div
            float=`right
            styles=[Css.style([FC.PageCard.HeaderRow.Styles.itemTopPadding])]>
            paginationPage
          </Div>
        </Div>
      </div>;

    <LeaderboardMembers
      agentId
      head
      columns=LeaderboardTable.Columns.members'
    />;
  },
};
