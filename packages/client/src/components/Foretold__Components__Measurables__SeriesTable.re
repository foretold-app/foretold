open Context.Primary;

let component = ReasonReact.statelessComponent("MeasurablesSeriesTable");

module MeasurableItems = Foretold__Components__Measurable__Items;
module Shared = Foretold__Components__Shared;

module Styles = {
  open Css;

  let row = selected =>
    style([
      width(`percent(100.0)),
      borderBottom(`px(1), `solid, hex("eee")),
      display(`flex),
      flexDirection(`row),
      paddingLeft(`px(8)),
      paddingRight(`px(8)),
      paddingTop(`px(8)),
      paddingBottom(`px(7)),
      cursor(`pointer),
      backgroundColor(`hex(selected ? "dce4ef" : "fff")),
      selector(":last-child", [borderBottom(`px(0), `solid, hex("eee"))]),
      selector(":hover", [backgroundColor(`hex("eef0f3"))]),
    ]);

  let column = style([flex(1)]);
  let group =
    style([
      border(`px(1), `solid, hex("eee")),
      backgroundColor(hex("fafafa")),
    ]);
};

let make =
    (
      ~measurables: array(Measurable.t),
      ~selected: option(string),
      ~onClick,
      _children,
    ) => {
  ...component,
  render: _self =>
    <div className=Styles.group>
      {
        measurables
        |> Array.map((m: Measurable.t) =>
             <div
               className={Styles.row(Some(m.id) == selected)}
               onClick={_e => onClick(m.id)}>
               <div className=Styles.column>
                 {
                   MeasurableItems.MeasurableEntityLinks.nameEntityLink(
                     ~m,
                     ~className=Shared.TagLink.item,
                   )
                   |> E.O.React.defaultNull
                 }
               </div>
               <div className=Styles.column>
                 {
                   MeasurableItems.MeasurableEntityLinks.propertyEntityLink(
                     ~m,
                     ~className=Shared.TagLink.property,
                   )
                   |> E.O.React.defaultNull
                 }
               </div>
               <div className=Styles.column>
                 {
                   MeasurableItems.dateItem(~m, ~showOn=false, ())
                   |> E.O.React.defaultNull
                 }
               </div>
               <div className=Styles.column>
                 {MeasurableItems.measurements(~m) |> E.O.React.defaultNull}
                 {MeasurableItems.measurers(~m) |> E.O.React.defaultNull}
               </div>
               <div className=Styles.column>
                 <Foretold__Components__Measurable.StatusDisplay
                   measurable=m
                 />
               </div>
             </div>
           )
        |> ReasonReact.array
      }
    </div>,
};