open FC__Settings.Text;

let activeStyles =
  Css.[
    borderBottom(`px(2), `solid, LightBackground.active),
    color(LightBackground.active),
    hover([color(LightBackground.active)]),
  ];

let inactiveStyles =
  Css.[
    borderBottom(`px(2), `solid, FC__Settings.clear),
    color(LightBackground.main),
    hover([color(LightBackground.active)]),
  ];

let allStyles =
  Css.[
    paddingBottom(`em(0.5)),
    paddingTop(`em(0.5)),
    paddingLeft(`em(0.4)),
    paddingRight(`em(0.4)),
    marginRight(`em(1.8)),
    FC__BaseStyles.floatLeft,
  ];

let flexStyles =
  Css.[textAlign(`center), flexGrow(1.), padding2(~v=`em(0.8), ~h=`zero)];

module Button = {
  open Css;
  let noStyles = [
    borderWidth(`px(0)),
    backgroundColor(`transparent),
    userSelect(`none),
    cursor(`pointer),
  ];

  let component = ReasonReact.statelessComponent("TabButton");
  let make = (~isActive=false, ~onClick=?, ~flex=false, children) => {
    ...component,
    render: _self =>
      <button
        disabled=isActive
        ?onClick
        className={Css.style(
          noStyles
          @ (isActive ? activeStyles : inactiveStyles)
          @ (flex ? flexStyles : allStyles),
        )}>
        ...children
      </button>,
  };
};

let component = ReasonReact.statelessComponent("Tab");
let make = (~isActive=false, ~onClick=?, ~flex=false, children) => {
  ...component,
  render: _self =>
    <FC__Link.Jsx2
      isDisabled=false
      ?onClick
      className={Css.style(
        (isActive ? activeStyles : inactiveStyles)
        @ (flex ? flexStyles : allStyles),
      )}>
      ...children
    </FC__Link.Jsx2>,
};