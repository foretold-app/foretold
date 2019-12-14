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

  let column = style([flex(`num(1.))]);
  let group =
    style([
      border(`px(1), `solid, hex("eee")),
      backgroundColor(hex("fafafa")),
    ]);
};

[@react.component]
let make =
    (
      ~measurables: array(Types.measurable),
      ~selected: option(string),
      ~onClick,
    ) => {
  <Providers.AppContext.Consumer>
    ...{context => {
      module Config = {
        let globalSetting = context.globalSetting;
      };
      module Ken = KenTools.Functor(Config);
      module MeasurableEntityLinks = MeasurableEntityLinks.Functor(Ken);

      <div className=Styles.group>
        {measurables
         |> Array.map((m: Types.measurable) =>
              <div
                className={Styles.row(Some(m.id) == selected)}
                onClick={_e => onClick(m.id)}>
                <div className=Styles.column>
                  {MeasurableEntityLinks.nameEntityLink(
                     ~m,
                     ~className=Shared.TagLink.item,
                   )
                   |> E.O.React.defaultNull}
                </div>
                <div className=Styles.column>
                  {MeasurableEntityLinks.propertyEntityLink(
                     ~m,
                     ~className=Shared.TagLink.property,
                   )
                   |> E.O.React.defaultNull}
                </div>
                <div className=Styles.column>
                  {MeasurableItems.dateItem(~m, ~showOn=false, ())
                   |> E.O.React.defaultNull}
                </div>
                <div className=Styles.column>
                  {MeasurableItems.measurements(~m) |> E.O.React.defaultNull}
                  {MeasurableItems.measurers(~m) |> E.O.React.defaultNull}
                </div>
                <div className=Styles.column>
                  <StatusDisplay measurable=m />
                </div>
              </div>
            )
         |> ReasonReact.array}
      </div>;
    }}
  </Providers.AppContext.Consumer>;
};