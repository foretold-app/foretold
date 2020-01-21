open Style.Grid;

module Pagination = {
  [@react.component]
  let make =
      (
        ~channelId: option(string),
        ~paginationPage,
        ~subTab: Routing.ChannelPage.leaderboard,
      ) => {
    <Div>
      <Div float=`left>
        <Div
          float=`left
          styles=[
            Css.style([
              ForetoldComponents.PageCard.HeaderRow.Styles.itemTopPadding,
              ForetoldComponents.PageCard.HeaderRow.Styles.itemBottomPadding,
            ]),
          ]>
          <ForetoldComponents.Tab2
            isActive={subTab === ByMember}
            onClick={LinkType.onClick(
              Internal(
                ChannelLeaderboard(channelId |> E.O.toString, ByMember),
              ),
            )}>
            {"Member Scores" |> Utils.ste}
          </ForetoldComponents.Tab2>
          <ForetoldComponents.Tab2
            isActive={subTab === ByMeasurable}
            onClick={LinkType.onClick(
              Internal(
                ChannelLeaderboard(channelId |> E.O.toString, ByMeasurable),
              ),
            )}>
            {"Question Scores" |> Utils.ste}
          </ForetoldComponents.Tab2>
        </Div>
      </Div>
      <Div
        float=`right
        styles=[
          Css.style([
            ForetoldComponents.PageCard.HeaderRow.Styles.itemTopPadding,
            ForetoldComponents.PageCard.HeaderRow.Styles.itemBottomPadding,
          ]),
        ]>
        paginationPage
      </Div>
    </Div>;
  };
};

let head =
    (
      ~channelId,
      ~subTab: Routing.ChannelPage.leaderboard=ByMeasurable,
      ~paginationPage,
      (),
    ) =>
  <Pagination channelId paginationPage subTab />;