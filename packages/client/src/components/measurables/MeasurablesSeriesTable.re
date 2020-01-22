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
  let context = React.useContext(Providers.app);
  module Config = {
    let globalSetting = context.globalSetting;
  };
  module Ken = KenTools.Functor(Config);
  module MeasurableEntityLinks = MeasurableEntityLinks.Functor(Ken);

  <div className=Styles.group>
    {measurables
     |> Array.map((measurable: Types.measurable) =>
          <div
            className={Styles.row(Some(measurable.id) == selected)}
            onClick={_e => onClick(measurable.id)}>
            <div className=Styles.column>
              {MeasurableEntityLinks.nameEntityLink(
                 ~m=measurable,
                 ~className=Shared.TagLink.item,
               )
               |> E.O.React.defaultNull}
            </div>
            <div className=Styles.column>
              {MeasurableEntityLinks.propertyEntityLink(
                 ~m=measurable,
                 ~className=Shared.TagLink.property,
               )
               |> E.O.React.defaultNull}
            </div>
            <div className=Styles.column>
              <MeasurableItems.DateItem measurable showOn=false />
            </div>
            <div className=Styles.column>
              <MeasurableItems.Measurements measurable />
              <MeasurableItems.Measurers measurable />
            </div>
            <div className=Styles.column> <StatusDisplay measurable /> </div>
          </div>
        )
     |> ReasonReact.array}
  </div>;
};