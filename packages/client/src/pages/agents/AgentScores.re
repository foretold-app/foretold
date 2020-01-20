open Style.Grid;

[@react.component]
let make = (~agentId) => {
  let head = (~channelId, ~paginationPage, ()) =>
    <div>
      <ForetoldComponents.Base.Div float=`left>
        <ForetoldComponents.PageCard.HeaderRow.Title>
          {"Scores" |> Utils.ste}
        </ForetoldComponents.PageCard.HeaderRow.Title>
      </ForetoldComponents.Base.Div>
      <Div>
        <Div
          float=`right
          styles=[
            Css.style([
              ForetoldComponents.PageCard.HeaderRow.Styles.itemTopPadding,
            ]),
          ]>
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