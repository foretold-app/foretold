[@bs.config {jsx: 3}];

type tab =
  | Measurements
  | Scores;

module Tabs = {
  open Style.Grid;
  open FC;

  [@react.component]
  let make = (~paginationPage, ~switchTab, ~tab) => {
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
            styles=[Css.style([FC.PageCard.HeaderRow.Styles.itemTopPadding])]>
            paginationPage
          </Div>
        </Div>
      </Div>
    </>;
  };
};

[@react.component]
let make = (~measurableId: string, ~channelId: option(string)) => {
  let (tab, setTab) = React.useState(() => Measurements);

  let switchTab = tabToSwitch => setTab(_ => tabToSwitch);
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
};