open Style.Grid;

let pagination =
    (
      channelId: option(string),
      paginationPage,
      subTab: Routing.ChannelPage.leaderboard,
    ) =>
  <Div>
    <Div float=`left>
      <Div
        float=`left
        styles=[
          Css.style([
            Fc.FC.PageCard.HeaderRow.Styles.itemTopPadding,
            Fc.FC.PageCard.HeaderRow.Styles.itemBottomPadding,
          ]),
        ]>
        <Fc.FC.Tab2
          isActive={subTab === ByMember}
          onClick={LinkType.onClick(
            Internal(
              ChannelLeaderboard(channelId |> E.O.toString, ByMember),
            ),
          )}>
          {"Member Scores" |> Utils.ste}
        </Fc.FC.Tab2>
        <Fc.FC.Tab2
          isActive={subTab === ByMeasurable}
          onClick={LinkType.onClick(
            Internal(
              ChannelLeaderboard(channelId |> E.O.toString, ByMeasurable),
            ),
          )}>
          {"Question Scores" |> Utils.ste}
        </Fc.FC.Tab2>
        <Fc.FC.Tab2
          isActive={subTab === ByMeasurement}
          onClick={LinkType.onClick(
            Internal(
              ChannelLeaderboard(channelId |> E.O.toString, ByMeasurement),
            ),
          )}>
          {"Prediction Scores" |> Utils.ste}
        </Fc.FC.Tab2>
      </Div>
    </Div>
    <Div
      float=`right
      styles=[
        Css.style([
          Fc.FC.PageCard.HeaderRow.Styles.itemTopPadding,
          Fc.FC.PageCard.HeaderRow.Styles.itemBottomPadding,
        ]),
      ]>
      paginationPage
    </Div>
  </Div>;