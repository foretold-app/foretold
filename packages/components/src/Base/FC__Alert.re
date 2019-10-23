[@bs.config {jsx: 3}];

/* For the icons, font-awesome (or similar?) is a possibility */

module Styles = {
  open Css;
  let alertBox =
    style([
      borderRadius(FC__Settings.BorderRadius.tight),
      padding2(~v=`em(0.5), ~h=`em(0.8)),
      marginBottom(`em(0.75)),
    ]);

  // Colors from https://getbootstrap.com/docs/4.0/components/alerts/
  // They may look better on white background than grey/smokeWhite
  let colors = (t: FC__Settings.Alert.t) =>
    style([
      color(FC__Settings.Alert.color(t)),
      backgroundColor(FC__Settings.Alert.background(t)),
    ]);
};

/**
 * Primary - Communicate information like "Welcome, now you can do..."
 * Info - Less significant information
 * Success
 * Warning
 * Error
 */

type type_ = [ | `primary | `info | `success | `warning | `error];

[@react.component]
let make = (~type_: type_=`info, ~children) => {
  let classes = Styles.alertBox ++ " " ++ Styles.colors(type_);
  <div className=classes> {children |> ReasonReact.array} </div>;
};

module Jsx2 = {
  let make = (~type_: type_=`info, children) =>
    ReasonReactCompat.wrapReactForReasonReact(
      make,
      makeProps(~type_, ~children, ()),
      children,
    );
};
