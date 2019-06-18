open FC__Colors.Text;

let activeStyles =
  Css.[
    borderBottom(`px(2), `solid, LightBackground.active),
    color(LightBackground.active),
    hover([color(LightBackground.active)]),
  ];
let inactiveStyles =
  Css.[
    borderBottom(`px(2), `solid, FC__Colors.clear),
    color(LightBackground.main),
    hover([color(LightBackground.active)]),
  ];
let allStyles =
  Css.[
    paddingBottom(`em(0.8)),
    paddingTop(`em(0.8)),
    paddingLeft(`em(0.4)),
    paddingRight(`em(0.4)),
    marginRight(`em(1.8)),
    FC__BaseStyles.floatLeft,
  ];

let flexStyles =
  Css.[textAlign(`center), flexGrow(1.), padding2(~v=`em(0.8), ~h=`zero)];

let component = ReasonReact.statelessComponent("Tab");
let make = (~isActive=false, ~onClick=?, ~flex=false, children) => {
  ...component,
  render: _self =>
    <FC__Link
      isDisabled=false
      ?onClick
      className={Css.style(
        (isActive ? activeStyles : inactiveStyles)
        @ (flex ? flexStyles : allStyles),
      )}>
      ...children
    </FC__Link>,
};
