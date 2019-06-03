/* For the icons, font-awesome (or similar?) is a possibility */

module Styles = {
  open Css;
  let alertBox =
    style([
      borderRadius(`px(4)),
      padding2(~v=`em(1.0), ~h=`em(0.8)),
      marginBottom(`em(0.75)),
      fontWeight(`bold),
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
};

/**
 * Primary - Communicate information like "Welcome, now you can do..."
 * Info - Less significant information
 * Success
 * Warning
 * Error
 */
type alertType =
  | Primary
  | Info
  | Success
  | Warning
  | Error;

let component = ReasonReact.statelessComponent(__MODULE__);

let make = (~type_=Info, children) => {
  ...component,
  render: _self => {
    let classes =
      Styles.alertBox
      ++ " "
      ++ (
        switch (type_) {
        | Primary => Styles.primaryStyle
        | Info => Styles.infoStyle
        | Success => Styles.successStyle
        | Warning => Styles.warningStyle
        | Error => Styles.errorStyle
        }
      );
    <div className=classes> ...children </div>;
  },
};
