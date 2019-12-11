open Style.Grid;

[@react.component]
let make = (~agentId) => {
  let head = (~channelId, ~paginationPage, ()) =>
    <div>
      <FC.Base.Div float=`left>
        <FC.PageCard.HeaderRow.Title>
          {"Scores" |> Utils.ste}
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

  <LeaderboardMembers
    agentId
    head
    columns=LeaderboardTable.Columns.members'
  />;
};