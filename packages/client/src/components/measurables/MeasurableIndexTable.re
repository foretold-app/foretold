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
        <FC.Table.HeaderRow>
          <FC.Table.Cell flex={`num(3.)} properties=Table.headerCellStyles>
            {"Name & Status" |> Utils.ste}
          </FC.Table.Cell>
          <FC.Table.Cell flex={`num(1.5)} properties=Table.headerCellStyles>
            {"Aggregate and resolution" |> Utils.ste}
          </FC.Table.Cell>
          <FC.Table.Cell flex={`num(1.)} properties=Table.headerCellStyles>
            {"Details" |> Utils.ste}
          </FC.Table.Cell>
        </FC.Table.HeaderRow>
        {measurables
         |> E.A.fmap((m: Types.measurable) =>
              <FC.Table.Row onClick={_e => onSelect(m)} key={m.id}>
                <FC.Table.Cell
                  flex={`num(3.)}
                  className=Css.(
                    style([paddingTop(`em(0.3)), paddingBottom(`em(0.3))])
                  )>
                  <div className=Styles.mainColumn>
                    <div className=Styles.mainColumnTop>
                      <MeasurableItems.LinkMeasurable m />
                    </div>
                  </div>
                  <div className=Styles.rightColumn>
                    <StatusDisplay measurable=m />
                  </div>
                </FC.Table.Cell>
                <FC.Table.Cell
                  flex={`num(1.5)}
                  className=Css.(style([paddingTop(`em(0.5))]))>
                  <MeasurementItems.ResolutionOrRecentAggregation
                    measurable=m
                    xMin=None
                    xMax=None
                  />
                </FC.Table.Cell>
                <FC.Table.Cell
                  flex={`num(1.)}
                  className=Css.(style([paddingTop(`em(0.5))]))>
                  {E.React2.showIf(
                     channelId == Some("home"),
                     <MeasurableItems.ChannelLink m />,
                   )}
                  {E.React2.showIf(
                     showExtraData,
                     <MeasurableItems.Series m />,
                   )}
                  {E.React2.showIf(
                     showExtraData,
                     <MeasurableItems.CreatorLink m />,
                   )}
                  <MeasurableItems.Measurements m />
                  <MeasurableItems.Measurers m />
                </FC.Table.Cell>
              </FC.Table.Row>
            )
         |> ReasonReact.array}
      </>
    : <NothingToShow />;
};