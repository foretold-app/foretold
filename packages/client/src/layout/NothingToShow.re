[@bs.config {jsx: 3}];

open Utils;

module Styles = {
  open Css;
  let centerBlock = style([textAlign(`center), margin(`em(2.))]);
  let clear = style([clear(`both)]);
};

[@react.component]
let make = () => {
  <>
    <div className=Styles.clear />
    <div className=Styles.centerBlock> {"No Results" |> Utils.ste} </div>
  </>;
};
