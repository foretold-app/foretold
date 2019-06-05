/* For the icons, font-awesome (or similar?) is a possibility */

module Styles = {
  open Css;
  let alertBox =
    style([
      borderRadius(FC__Colors.BorderRadius.tight),
      padding2(~v=`em(0.5), ~h=`em(0.8)),
      marginBottom(`em(0.75)),
    ]);

  // Colors from https://getbootstrap.com/docs/4.0/components/alerts/
  // They may look better on white background than grey/smokeWhite
  let colors = (t: FC__Colors.Alert.t) =>
    style([
      color(FC__Colors.Alert.color(t)),
      backgroundColor(FC__Colors.Alert.background(t)),
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

let component = ReasonReact.statelessComponent(__MODULE__);

let make = (~type_: type_=`info, children) => {
  ...component,
  render: _self => {
    let classes = Styles.alertBox ++ " " ++ Styles.colors(type_);
    <div className=classes> ...children </div>;
  },
};