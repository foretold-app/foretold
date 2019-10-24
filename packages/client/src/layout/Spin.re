[@bs.config {jsx: 3}];

open Utils;

module Styles = {
  open Css;
  let centerBlock = style([textAlign(`center), padding(`em(2.))]);
  let clear = style([clear(`both)]);
};

// @todo: 1
[@react.component]
let make = () =>
  <>
    <div className=Styles.clear />
    <div className=Styles.centerBlock>
      {"<Antd.Spin tip=\"Loading...\" />" |> Utils.ste}
    </div>
  </>;
