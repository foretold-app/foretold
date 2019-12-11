module Styles = {
  open Css;
  let main = style([padding(`em(2.))]);
};

[@react.component]
let make = (~children) => <> <div className=Styles.main> children </div> </>;