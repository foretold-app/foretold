[@bs.config {jsx: 3}];

open Css;

let widthStyle = (~fullWidth=false, ()) => {
  let formInputInset =
    style([
      border(`px(1), `solid, FC__Settings.border),
      borderRadius(`px(4)),
      padding(`em(0.6)),
      boxShadows([`shadow("inset 0 1px 2px rgba(0,0,0.7,0.08)")]),
      fontSize(`em(1.0)),
    ]);

  let inputFullWidth =
    style([width(`percent(100.)), boxSizing(`borderBox)]);

  let textAreaFullWidth = merge([formInputInset, inputFullWidth]);
  fullWidth ? textAreaFullWidth : formInputInset;
};
