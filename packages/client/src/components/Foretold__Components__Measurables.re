module Items = Foretold__Components__Measurable__Items;
module SeriesTable = Foretold__Components__Measurables__SeriesTable;

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

module BasicTable = {
  let component = ReasonReact.statelessComponent("MeasurableIndexTable");

  let make =
      (
        ~measurables: array(Types.measurable),
        ~showExtraData: bool,
        ~onSelect=(_m: Types.measurable) => (),
        ~channelId: option(string)=None,
        _children,
      ) => {
    ...component,
    render: _self =>
      measurables |> E.A.length > 0
        ? <>
            <FC.Table.HeaderRow>
              <FC.Table.Cell flex={`num(3.)}>
                {"Name & Status" |> ReasonReact.string}
              </FC.Table.Cell>
              <FC.Table.Cell flex={`num(1.)}>
                {"Details" |> ReasonReact.string}
              </FC.Table.Cell>
            </FC.Table.HeaderRow>
            {measurables
             |> E.A.fmap((m: Types.measurable) => {
                  let iAmOwner = m.iAmOwner == Some(true);
                  <FC.Table.Row onClick={_e => onSelect(m)} key={m.id}>
                    <FC.Table.Cell
                      flex={`num(3.)}
                      className=Css.(
                        style([
                          paddingTop(`em(0.3)),
                          paddingBottom(`em(0.3)),
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
                      flex={`num(1.)}
                      className=Css.(style([paddingTop(`em(0.5))]))>
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
                  </FC.Table.Row>;
                })
             |> ReasonReact.array}
          </>
        : <SLayout.NothingToShow />,
  };
};