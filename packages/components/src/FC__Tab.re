open FC__Base;
open Colors.Text;

let activeStyles =
  Css.[borderBottom(`px(2), `solid, LightBackground.active)];
let activeColors = (LightBackground.active, LightBackground.active);
let inactiveStyles =
  Css.[
    borderBottom(`px(2), `solid, Colors.clear),
    color(LightBackground.main),
  ];
let inactiveColors = (LightBackground.main, LightBackground.active);
let allStyles =
  Css.[
    paddingBottom(`em(0.8)),
    paddingTop(`em(0.8)),
    paddingLeft(`em(0.4)),
    paddingRight(`em(0.4)),
    marginRight(`em(1.8)),
    BaseStyles.floatLeft,
  ];

let component = ReasonReact.statelessComponent("Tab");
let make = (~isActive=false, children) => {
  ...component,
  render: _self =>
    <Link
      colors={isActive ? activeColors : inactiveColors}
      isDisabled=false
      styles=Css.((isActive ? activeStyles : inactiveStyles) @ allStyles)>
      ...children
    </Link>,
};