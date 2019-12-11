module Styles = {
  open Css;
  let centerBlock = style([textAlign(`center), padding(`em(2.))]);
  let clear = style([clear(`both)]);
};

[@react.component]
let make = (~children) =>
  <>
    <div className=Styles.clear />
    <div className=Styles.centerBlock> children </div>
  </>;