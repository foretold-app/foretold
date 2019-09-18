open Style.Grid;

let component = ReasonReact.statelessComponent("PaginationX");
let make =
    (
      ~paginationPage,
      ~channelId: option(string),
      ~measurableId,
      ~subTab: Routing.ChannelPage.measurementsTab=ByMeasurements,
      _children,
    ) => {
  ...component,
  render: _ =>
    <>
      <Div>
        <Div
          styles=[
            Css.style([
              FC.Base.BaseStyles.floatLeft,
              Css.paddingTop(`em(0.2)),
            ]),
          ]>
          <FC.Tab
            isActive={subTab == ByMeasurements}
            onClick={LinkType.onClick(
              Internal(
                MeasurableShow(
                  channelId |> E.O.toString,
                  measurableId,
                  ByMeasurements,
                ),
              ),
            )}>
            {"Predictions" |> ReasonReact.string}
          </FC.Tab>
          <FC.Tab
            isActive={subTab == ByScores}
            onClick={LinkType.onClick(
              Internal(
                MeasurableShow(
                  channelId |> E.O.toString,
                  measurableId,
                  ByScores,
                ),
              ),
            )}>
            {"Scores" |> ReasonReact.string}
          </FC.Tab>
        </Div>
        <Div>
          <Div
            float=`right
            styles=[Css.style([FC.PageCard.HeaderRow.Styles.itemTopPadding])]>
            paginationPage
          </Div>
        </Div>
      </Div>
    </>,
};