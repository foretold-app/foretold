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
    <div className=Styles.centerBlock>
      <Icon icon="MAGIC_WAND" size="2em" />
      <p />
      <div> {Lang.networkError |> Utils.ste} </div>
      <div> {Lang.tryToChangeInput |> Utils.ste} </div>
    </div>
  </>;
};