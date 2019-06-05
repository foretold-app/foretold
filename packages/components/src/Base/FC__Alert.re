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
  let primaryStyle =
    style([color(`hex("004085")), backgroundColor(`hex("cce5ff"))]);
  let infoStyle =
    style([color(`hex("0c5460")), backgroundColor(`hex("d1ecf1"))]);
  let successStyle =
    style([color(`hex("155724")), backgroundColor(`hex("d4edda"))]);
  let warningStyle =
    style([color(`hex("856404")), backgroundColor(`hex("fff3cd"))]);
  let errorStyle =
    style([color(`hex("721c24")), backgroundColor(`hex("f8d7da"))]);
  
  let itemStyle = (t: FC__Colors.Alert.t) => style([color(FC__Colors.Alert.color(t)), backgroundColor(FC__Colors.Alert.background(t))]);
};

/**
 * Primary - Communicate information like "Welcome, now you can do..."
 * Info - Less significant information
 * Success
 * Warning
 * Error
 */

let component = ReasonReact.statelessComponent(__MODULE__);

let make = (~type_:FC__Colors.Alert.t=Info, children) => {
  ...component,
  render: _self => {
    let classes =
      Styles.alertBox
      ++ " "
      ++ Styles.itemStyle(type_);
    <div className=classes> ...children </div>;
  },
};
