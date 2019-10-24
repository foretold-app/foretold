type tab =
  | Measurements
  | Scores;

type state = {tab};

type action =
  | SwitchTab(tab);

module Tabs = {
  open Style.Grid;
  open FC;
  let component = ReasonReact.statelessComponent("Tabs");
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
            <TabButton
              isActive={tab == Measurements}
              onClick={_ => switchTab(Measurements)}>
              {"Predictions" |> ReasonReact.string}
            </TabButton>
            <TabButton
              isActive={tab == Scores} onClick={_ => switchTab(Scores)}>
              {"Scores" |> ReasonReact.string}
            </TabButton>
          </Div>
          <Div>
            <Div
              float=`right
              styles=[
                Css.style([FC.PageCard.HeaderRow.Styles.itemTopPadding]),
              ]>
              paginationPage
            </Div>
          </Div>
        </Div>
      </>,
  };
};

let component = ReasonReact.reducerComponent("MeasurableBottomSection");
let make = (~measurableId: string, ~channelId: option(string), _children) => {
  ...component,
  reducer: (action, _state) =>
    switch (action) {
    | SwitchTab(tab) => ReasonReact.Update({tab: tab})
    },

  initialState: () => {tab: Measurements},
  render: self => {
    let tab = self.state.tab;
    let switchTab = tabToSwitch => self.send(SwitchTab(tabToSwitch));
    let head = (~channelId: option(string), ~paginationPage, ()) =>
      <Tabs switchTab paginationPage tab />;

    switch (tab) {
    | Measurements => <Measurements measurableId head />
    | Scores =>
      <LeaderboardMeasurables
        channelId
        measurableId={Some(measurableId)}
        columns=LeaderboardTable.Columns.measurables'
        head
      />
    };
  },
};
