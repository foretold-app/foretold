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
            FC.PageCard.HeaderRow.Styles.itemTopPadding,
            FC.PageCard.HeaderRow.Styles.itemBottomPadding,
          ]),
        ]>
        <FC.Tab2
          isActive={subTab === ByMember}
          onClick={LinkType.onClick(
            Internal(
              ChannelLeaderboard(channelId |> E.O.toString, ByMember),
            ),
          )}>
          {"By Member" |> Utils.ste}
        </FC.Tab2>
        <FC.Tab2
          isActive={subTab === ByMeasurement}
          onClick={LinkType.onClick(
            Internal(
              ChannelLeaderboard(channelId |> E.O.toString, ByMeasurement),
            ),
          )}>
          {"By Prediction" |> Utils.ste}
        </FC.Tab2>
        <FC.Tab2
          isActive={subTab === ByMeasurable}
          onClick={LinkType.onClick(
            Internal(
              ChannelLeaderboard(channelId |> E.O.toString, ByMeasurable),
            ),
          )}>
          {"By Question" |> Utils.ste}
        </FC.Tab2>
      </Div>
    </Div>
    <Div
      float=`right
      styles=[
        Css.style([
          FC.PageCard.HeaderRow.Styles.itemTopPadding,
          FC.PageCard.HeaderRow.Styles.itemBottomPadding,
        ]),
      ]>
      paginationPage
    </Div>
  </Div>;