[@bs.config {jsx: 3}];

let inputHeader =
  Css.(style([paddingLeft(`em(0.4)), paddingBottom(`em(0.6))]));

[@react.component]
let make = (~children) => <div className=inputHeader> children </div>;

module Jsx2 = {
  let make = children =>
    ReasonReactCompat.wrapReactForReasonReact(
      make,
      makeProps(~children, ()),
      children,
    );
};
