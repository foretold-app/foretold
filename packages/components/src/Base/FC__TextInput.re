let component = ReasonReact.statelessComponent(__MODULE__);

module Styles = {
  open Css;

  let inputHeader =
    style([
      fontSize(`px(14)),
      fontWeight(`bold),
      paddingLeft(`em(0.4)),
      paddingBottom(`em(0.6)),
    ]);

  let formInputInset =
    style([
      border(`px(1), `solid, FC__Colors.border),
      borderRadius(`px(4)),
      padding(`em(0.6)),
      boxShadows([`shadow("inset 0 1px 2px rgba(0,0,0.7,0.08)")]),
    ]);

  let inputFullWidth =
    style([width(`percent(100.)), boxSizing(`borderBox)]);

  let textInputFullWidth = merge([formInputInset, inputFullWidth]);
};

let make = (~fullWidth=false, ~placeholder=?, _children) => {
  ...component,
  render: _self =>
    <input
      type_="text"
      className={fullWidth ? Styles.textInputFullWidth : Styles.formInputInset}
      ?placeholder
    />,
};
