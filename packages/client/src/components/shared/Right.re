[@bs.config {jsx: 3}];

module Styles = {
  open Css;
  let rightBlock = style([textAlign(`right)]);
};

[@react.component]
let make = (~children) => <div className=Styles.rightBlock> children </div>;