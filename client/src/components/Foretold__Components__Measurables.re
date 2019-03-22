open Utils;
open Foretold__GraphQL;
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
    let statusOpacity = (measurable: DataModel.Measurable.t) => {
      let state = measurable.state |> E.O.toExn("Needs state from GraphQL");
      if (state === `ARCHIVED) {
        0.8;
      } else if (state === `JUDGED) {
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
    style([flex(5), display(`flex), flexDirection(`column)]);

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
        ~measurables: array(DataModel.Measurable.t),
        ~showExtraData: bool,
        ~loggedInUser: DataModel.User.t,
        ~onSelect=(m: DataModel.Measurable.t) => (),
        _children,
      ) => {
    ...component,
    render: _self =>
      <div className=Styles.group>
        {
          measurables
          |> E.A.fmap((m: DataModel.Measurable.t) => {
               open Rationale.Option.Infix;
               let userAgentId = loggedInUser.agent <$> (r => r.id);
               let measurableAgentId =
                 m.creator |> E.O.fmap((r: DataModel.Agent.t) => r.id);
               let isSame =
                 userAgentId == measurableAgentId && E.O.isSome(userAgentId);
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
                         E.React.showIf(isSame, Items.editLink(~m)),
                         E.React.showIf(isSame, Items.archiveOption(~m)),
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
  };
};