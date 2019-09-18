open Style.Grid;

let component = ReasonReact.statelessComponent("PaginationX");
let make =
    (
      ~paginationPage,
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
          <FC.Tab isActive={subTab == ByMeasurements}>
            {"Predictions" |> ReasonReact.string}
          </FC.Tab>
          <FC.Tab isActive={subTab == ByScores}>
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