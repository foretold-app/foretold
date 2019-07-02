module Items = Foretold__Components__Measurable__Items;
module SeriesTable = Foretold__Components__Measurables__SeriesTable;

module Styles = {
  open Css;

  let mainColumn =
    style([flex(4), display(`flex), flexDirection(`column)]);

  let mainColumnTop =
    style([
      flex(1),
      paddingLeft(px(2)),
      selector(" p", [marginTop(`px(3)), marginBottom(`px(8))]),
    ]);
  let rightColumn = style([flex(1)]);
  let mainColumnBottom =
    style([flex(1), padding(`px(2)), marginTop(`px(2))]);
};

module BasicTable = {
  let component = ReasonReact.statelessComponent("MeasurableIndexTable");

  let make =
      (
        ~measurables: array(Primary.Measurable.t),
        ~showExtraData: bool,
        ~onSelect=(m: Primary.Measurable.t) => (),
        ~channelId: option(string)=None,
        _children,
      ) => {
    ...component,
    render: _self =>
      measurables |> E.A.length > 0
        ? <>
            <FC.Table.HeaderRow>
              <FC.Table.Cell flex=3>
                {"Name & Status" |> ReasonReact.string}
              </FC.Table.Cell>
              <FC.Table.Cell flex=1>
                {"Details" |> ReasonReact.string}
              </FC.Table.Cell>
            </FC.Table.HeaderRow>
            {measurables
             |> E.A.fmap((m: Primary.Measurable.t) => {
                  let iAmOwner = m.iAmOwner == Some(true);
                  <FC.Table.RowLink onClick={_e => onSelect(m)} key={m.id}>
                    <FC.Table.Cell
                      flex=3
                      className=Css.(
                        style([
                          paddingTop(`em(1.0)),
                          paddingBottom(`em(0.5)),
                        ])
                      )>
                      <div className=Styles.mainColumn>
                        <div className=Styles.mainColumnTop>
                          {Items.link(~m)}
                        </div>
                      </div>
                      <div className=Styles.rightColumn>
                        <Foretold__Components__Measurable.StatusDisplay
                          measurable=m
                        />
                      </div>
                    </FC.Table.Cell>
                    <FC.Table.Cell
                      flex=1 className=Css.(style([paddingTop(`em(0.5))]))>
                      {E.React.showIf(
                         channelId == Some("home"),
                         Items.channelLink(~m),
                       )}
                      {E.React.showIf(
                         showExtraData,
                         Items.series(~m, ~channelId, ())
                         |> E.O.React.defaultNull,
                       )}
                      {E.React.showIf(
                         showExtraData,
                         Items.creatorLink(~m) |> E.O.React.defaultNull,
                       )}
                      {Items.measurements(~m) |> E.O.React.defaultNull}
                      {Items.measurers(~m) |> E.O.React.defaultNull}
                      {E.React.showIf(iAmOwner, Items.editLink(~m))}
                    </FC.Table.Cell>
                  </FC.Table.RowLink>;
                  /* {E.React.showIf(iAmOwner, Items.archiveOption(~m))} */
                })
             |> ReasonReact.array}
          </>
        : <SLayout.NothingToShow />,
  };
};