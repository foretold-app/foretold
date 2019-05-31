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

  let textAreaFullWidth = merge([formInputInset, inputFullWidth]);
};

let make = (~fullWidth=false, ~rows=5, ~value=?, _children) => {
  ...component,
  render: _self =>
    <textarea
      className={fullWidth ? Styles.textAreaFullWidth : Styles.formInputInset}
      rows>
      {switch (value) {
       | Some(v) => v |> React.string
       | None => React.null
       }}
    </textarea>,
};