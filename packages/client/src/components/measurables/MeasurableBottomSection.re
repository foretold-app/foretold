type tab =
  | Measurements
  | Scores
  | Details
  | Edit;

module Styles = {
  open Css;
  let block = style([marginTop(`em(1.))]);
};

module Tabs = {
  open Style.Grid;

  [@react.component]
  let make =
      (~paginationPage, ~switchTab, ~tab, ~measurable: Types.measurable) => {
    <Div>
      <Div
        styles=[
          Css.style([
            ForetoldComponents.Base.BaseStyles.floatLeft,
            Css.paddingTop(`em(0.2)),
          ]),
        ]>
        <ForetoldComponents.Tab.Button
          isActive={tab == Measurements}
          onClick={_ => switchTab(Measurements)}>
          {"Predictions" |> Utils.ste}
        </ForetoldComponents.Tab.Button>
        <ForetoldComponents.Tab.Button
          isActive={tab == Scores} onClick={_ => switchTab(Scores)}>
          {"Scores" |> Utils.ste}
        </ForetoldComponents.Tab.Button>
        <ForetoldComponents.Tab.Button
          isActive={tab == Details} onClick={_ => switchTab(Details)}>
          {"Details" |> Utils.ste}
        </ForetoldComponents.Tab.Button>
        {<ForetoldComponents.Tab.Button
           isActive={tab == Edit} onClick={_ => switchTab(Edit)}>
           {"Edit" |> Utils.ste}
         </ForetoldComponents.Tab.Button>
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
          styles=[
            Css.style([
              ForetoldComponents.PageCard.HeaderRow.Styles.itemTopPadding,
            ]),
          ]>
          paginationPage
        </Div>
      </Div>
    </Div>;
  };
};

module Inner = {
  [@react.component]
  let make =
      (
        ~measurable,
        ~block=`none,
        ~leaderboardColumns=LeaderboardTable.Columns.measurables',
      ) => {
    let (tab, setTab) = React.useState(() => Measurements);

    let switchTab = tabToSwitch => setTab(_ => tabToSwitch);
    let head = (~channelId: option(string), ~paginationPage, ()) =>
      <Tabs switchTab paginationPage tab measurable />;

    switch (tab) {
    | Measurements => <Measurements measurableId={measurable.id} head block />

    | Scores =>
      <LeaderboardMeasurables
        head
        container=`none
        columns=leaderboardColumns
        finalComparisonMeasurement={
          measurable.state == Some(`JUDGED)
            ? Some(`LAST_OBJECTIVE_MEASUREMENT)
            : Some(`LAST_AGGREGATE_MEASUREMENT)
        }
        measurableId={Some(measurable.id)}
        channelId={Some(measurable.channelId)}
      />

    | Details =>
      <SLayout
        head={head(
          ~channelId=Some(measurable.channelId),
          ~paginationPage=E.React2.null,
          (),
        )}
        container=`none>
        <ForetoldComponents.PageCard.Body>
          <Style.Grid.Div
            float=`left
            styles=[
              Css.style([
                ForetoldComponents.PageCard.HeaderRow.Styles.itemTopPadding,
                ForetoldComponents.PageCard.HeaderRow.Styles.itemBottomPadding,
                Css.marginLeft(`em(1.0)),
              ]),
            ]>
            <ForetoldComponents.Div>
              <h4> {"Question Id:" |> Utils.ste} </h4>
              <div> <MeasurableItems.Id measurable /> </div>
            </ForetoldComponents.Div>
            <ForetoldComponents.Div styles=[Styles.block]>
              <h4> {"Name:" |> Utils.ste} </h4>
              <MeasurableItems.LinkMeasurable measurable />
            </ForetoldComponents.Div>
            <ForetoldComponents.Div styles=[Styles.block]>
              <h4> {"Description:" |> Utils.ste} </h4>
              <MeasurableItems.Description measurable />
            </ForetoldComponents.Div>
            <ForetoldComponents.Div styles=[Styles.block]>
              <h4> {"Owner:" |> Utils.ste} </h4>
              <div> <MeasurableItems.CreatorLink measurable /> </div>
            </ForetoldComponents.Div>
          </Style.Grid.Div>
        </ForetoldComponents.PageCard.Body>
      </SLayout>

    | Edit =>
      <SLayout
        head={head(
          ~channelId=Some(measurable.channelId),
          ~paginationPage=E.React2.null,
          (),
        )}
        container=`none>
        <ForetoldComponents.PageCard.Body>
          <Style.Grid.Div
            float=`left
            styles=[
              Css.style([
                ForetoldComponents.PageCard.HeaderRow.Styles.itemTopPadding,
                ForetoldComponents.PageCard.HeaderRow.Styles.itemBottomPadding,
                Css.marginLeft(`em(1.0)),
              ]),
            ]>
            <MeasurableEdit
              measurableId={measurable.id}
              key={"measurable-edit-" ++ measurable.id}
            />
          </Style.Grid.Div>
        </ForetoldComponents.PageCard.Body>
      </SLayout>
    };
  };
};

[@react.component]
let make = (~measurable) => {
  <SLayout.Container container=`fluid>
    <Inner measurable />
  </SLayout.Container>;
};