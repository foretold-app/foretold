[@bs.config {jsx: 3}];

type tab =
  | Measurements
  | Scores
  | Details;

module Tabs = {
  open Style.Grid;
  open FC;

  [@react.component]
  let make = (~paginationPage, ~switchTab, ~tab) => {
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
          {"Predictions" |> Utils.ste}
        </TabButton>
        <TabButton isActive={tab == Scores} onClick={_ => switchTab(Scores)}>
          {"Scores" |> Utils.ste}
        </TabButton>
        <TabButton
          isActive={tab == Details} onClick={_ => switchTab(Details)}>
          {"Details" |> Utils.ste}
        </TabButton>
      </Div>
      <Div>
        <Div
          float=`right
          styles=[Css.style([FC.PageCard.HeaderRow.Styles.itemTopPadding])]>
          paginationPage
        </Div>
      </Div>
    </Div>;
  };
};

[@react.component]
let make = (~measurableId: string, ~channelId: option(string)) => {
  let (tab, setTab) = React.useState(() => Measurements);

  let switchTab = tabToSwitch => setTab(_ => tabToSwitch);
  let head = (~channelId: option(string), ~paginationPage, ()) =>
    <Tabs switchTab paginationPage tab />;

  MeasurableGet.component(~id=measurableId)
  |> E.F.apply((measurable: Types.measurable) =>
       switch (tab) {
       | Measurements => <Measurements measurableId head />
       | Scores =>
         <LeaderboardMeasurables
           channelId
           measurableId={Some(measurableId)}
           columns=LeaderboardTable.Columns.measurables'
           finalComparisonMeasurement={
             measurable.state == Some(`JUDGED)
               ? Some(`LAST_OBJECTIVE_MEASUREMENT)
               : Some(`LAST_AGGREGATE_MEASUREMENT)
           }
           head
         />
       | Details =>
         <SLayout
           head={head(~channelId, ~paginationPage=E.React2.null, ())}
           isFluid=true>
           <FC.PageCard.Body>
             <Style.Grid.Div
               float=`left
               styles=[
                 Css.style([
                   FC.PageCard.HeaderRow.Styles.itemTopPadding,
                   FC.PageCard.HeaderRow.Styles.itemBottomPadding,
                   Css.marginLeft(`em(1.0)),
                 ]),
               ]>
               <h4> {"Question Id:" |> Utils.ste} </h4>
               {measurableId |> Utils.ste}
             </Style.Grid.Div>
           </FC.PageCard.Body>
         </SLayout>
       }
     );
};