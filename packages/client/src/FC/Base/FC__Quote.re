open FC__Base;
let quote =
  Css.(style(FC__Globals.commentStyle @ BaseStyles.fullWidthFloatLeft));

let component = ReasonReact.statelessComponent("Quote");

let make = children => {
  ...component,
  render: _self => <Div styles=[quote]> ...children </Div>,
};