[@bs.config {jsx: 3}];

[@react.component]
let make = (~fullWidth=false, ~placeholder=?) =>
  <input
    type_="text"
    className={FC__FormStyles.widthStyle(~fullWidth, ())}
    ?placeholder
  />;

module Jsx2 = {
  let make = (~fullWidth=false, ~placeholder=?, children) =>
    ReasonReactCompat.wrapReactForReasonReact(
      make,
      makeProps(~fullWidth, ~placeholder?, ()),
      children,
    );
};
