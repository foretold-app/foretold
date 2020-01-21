module Styles = {
  open Css;

  let mainColumn =
    style([flex(`num(4.)), display(`flex), flexDirection(`column)]);

  let mainColumnTop =
    style([
      flex(`num(1.)),
      paddingLeft(px(2)),
      selector(" p", [marginTop(`px(3)), marginBottom(`px(8))]),
    ]);
  let rightColumn = style([flex(`num(1.))]);
};

[@react.component]
let make =
    (
      ~measurables: array(Types.measurable),
      ~showExtraData: bool,
      ~onSelect=(_m: Types.measurable) => (),
      ~channelId=None,
    ) => {
  measurables |> E.A.length > 0
    ? <>
        <ForetoldComponents.Table.HeaderRow>
          <ForetoldComponents.Table.Cell
            flex={`num(3.)} properties=Table.headerCellStyles>
            {"Name & Status" |> Utils.ste}
          </ForetoldComponents.Table.Cell>
          <ForetoldComponents.Table.Cell
            flex={`num(1.5)} properties=Table.headerCellStyles>
            {"Aggregate and resolution" |> Utils.ste}
          </ForetoldComponents.Table.Cell>
          <ForetoldComponents.Table.Cell
            flex={`num(1.)} properties=Table.headerCellStyles>
            {"Details" |> Utils.ste}
          </ForetoldComponents.Table.Cell>
        </ForetoldComponents.Table.HeaderRow>
        {measurables
         |> E.A.fmap((measurable: Types.measurable) =>
              <ForetoldComponents.Table.Row
                onClick={_e => onSelect(measurable)} key={measurable.id}>
                <ForetoldComponents.Table.Cell
                  flex={`num(3.)}
                  className=Css.(
                    style([paddingTop(`em(0.3)), paddingBottom(`em(0.3))])
                  )>
                  <div className=Styles.mainColumn>
                    <div className=Styles.mainColumnTop>
                      <MeasurableItems.NameMeasurable measurable />
                    </div>
                  </div>
                  <div className=Styles.rightColumn>
                    <StatusDisplay measurable />
                  </div>
                </ForetoldComponents.Table.Cell>
                <ForetoldComponents.Table.Cell
                  flex={`num(1.5)}
                  className=Css.(style([paddingTop(`em(0.5))]))>
                  <MeasurementItems.ResolutionOrRecentAggregation
                    measurable
                    xMin=None
                    xMax=None
                  />
                </ForetoldComponents.Table.Cell>
                <ForetoldComponents.Table.Cell
                  flex={`num(1.)}
                  className=Css.(style([paddingTop(`em(0.5))]))>
                  {E.React2.showIf(
                     channelId == Some("home"),
                     <MeasurableItems.ChannelLink measurable />,
                   )}
                  {E.React2.showIf(
                     showExtraData,
                     <MeasurableItems.Series measurable />,
                   )}
                  {E.React2.showIf(
                     showExtraData,
                     <MeasurableItems.CreatorLink measurable />,
                   )}
                  <MeasurableItems.Measurements measurable />
                  <MeasurableItems.Measurers measurable />
                </ForetoldComponents.Table.Cell>
              </ForetoldComponents.Table.Row>
            )
         |> ReasonReact.array}
      </>
    : <NothingToShow />;
};