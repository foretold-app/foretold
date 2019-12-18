type tab =
  | Measurements
  | Scores
  | Details
  | Edit;

module Tabs = {
  open Style.Grid;
  open FC;

  [@react.component]
  let make =
      (~paginationPage, ~switchTab, ~tab, ~measurable: Types.measurable) => {
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
        {<TabButton isActive={tab == Edit} onClick={_ => switchTab(Edit)}>
           {"Edit" |> Utils.ste}
         </TabButton>
         |> E.React2.showIf(
              Primary.Permissions.can(
                `MEASURABLE_UPDATE,
                measurable.permissions,
              ),
            )}
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
let make = (~measurable: Types.measurable) => {
  let (tab, setTab) = React.useState(() => Measurements);

  let switchTab = tabToSwitch => setTab(_ => tabToSwitch);
  let head = (~channelId: option(string), ~paginationPage, ()) =>
    <Tabs switchTab paginationPage tab measurable />;

  switch (tab) {
  | Measurements => <Measurements measurableId={measurable.id} head />
  | Scores =>
    <LeaderboardMeasurables
      channelId={Some(measurable.channelId)}
      measurableId={Some(measurable.id)}
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
      head={head(
        ~channelId=Some(measurable.channelId),
        ~paginationPage=E.React2.null,
        (),
      )}
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
          {measurable.id |> Utils.ste}
        </Style.Grid.Div>
      </FC.PageCard.Body>
    </SLayout>

  | Edit =>
    <SLayout
      head={head(
        ~channelId=Some(measurable.channelId),
        ~paginationPage=E.React2.null,
        (),
      )}
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
          <MeasurableEdit measurableId={measurable.id} />
        </Style.Grid.Div>
      </FC.PageCard.Body>
    </SLayout>
  };
};