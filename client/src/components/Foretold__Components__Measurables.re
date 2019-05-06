module Items = Foretold__Components__Measurable__Items;
module SeriesTable = Foretold__Components__Measurables__SeriesTable;

module Styles = {
  open Css;
  let group =
    style([
      border(`px(1), `solid, hex("eee")),
      backgroundColor(hex("fafafa")),
    ]);

  let row = m => {
    let statusOpacity = (measurable: Context.Primary.Measurable.t) => {
      let state = measurable.state |> E.O.toExn("Needs state from GraphQL");
      if (state === `JUDGED) {
        0.55;
      } else {
        1.0;
      };
    };
    style([
      width(`percent(100.0)),
      borderBottom(`px(1), `solid, hex("eee")),
      selector(" h2", [marginTop(px(2))]),
      display(`flex),
      opacity(statusOpacity(m)),
      flexDirection(`row),
      paddingLeft(px(8)),
      paddingRight(px(8)),
      paddingTop(px(8)),
      paddingBottom(px(7)),
      cursor(`pointer),
      selector(":last-child", [borderBottom(`px(0), `solid, hex("eee"))]),
      selector(":hover", [backgroundColor(`hex("eef0f3"))]),
    ]);
  };
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
        ~measurables: array(Context.Primary.Measurable.t),
        ~showExtraData: bool,
        ~onSelect=(m: Context.Primary.Measurable.t) => (),
        _children,
      ) => {
    ...component,
    render: _self =>
      E.React.showIf(
        measurables |> E.A.length > 0,
        <div className=Styles.group>
          {
            measurables
            |> E.A.fmap((m: Context.Primary.Measurable.t) => {
                 let iAmOwner = m.iAmOwner == Some(true);
                 <div
                   className={Styles.row(m)}
                   onClick={_e => onSelect(m)}
                   key={m.id}>
                   <div className=Styles.mainColumn>
                     <div className=Styles.mainColumnTop>
                       {Items.link(~m)}
                     </div>
                     <div className=Styles.mainColumnBottom>
                       {
                         [|
                           E.React.showIf(
                             showExtraData,
                             Items.series(~m) |> E.O.React.defaultNull,
                           ),
                           E.React.showIf(
                             showExtraData,
                             Items.creatorLink(~m) |> E.O.React.defaultNull,
                           ),
                           Items.measurements(~m) |> E.O.React.defaultNull,
                           Items.measurers(~m) |> E.O.React.defaultNull,
                           E.React.showIf(iAmOwner, Items.editLink(~m)),
                           E.React.showIf(iAmOwner, Items.archiveOption(~m)),
                         |]
                         |> ReasonReact.array
                       }
                     </div>
                   </div>
                   <div className=Styles.rightColumn>
                     <Foretold__Components__Measurable.StatusDisplay
                       measurable=m
                       dateDisplay=WHOLE
                     />
                   </div>
                 </div>;
               })
            |> ReasonReact.array
          }
        </div>,
      ),
  };
};