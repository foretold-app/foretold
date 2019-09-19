open Style.Grid;

type tab =
  | Measurements
  | Scores;

let component = ReasonReact.statelessComponent("PaginationX");
let make = (~paginationPage, ~switchTab, ~tab, _children) => {
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
            isActive={tab == Measurements}
            onClick={_ => switchTab(Measurements)}>
            {"Predictions" |> ReasonReact.string}
          </FC.Tab>
          <FC.Tab isActive={tab == Scores} onClick={_ => switchTab(Scores)}>
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