open Utils;
open HandsOnTable;
let component = ReasonReact.statelessComponent("PaddedLayout");

module Styles = {
  open Css;
  let body = style([padding2(~v=px(10), ~h=px(50))]);
};

let make = children => {
  ...component,
  render: _ =>
    <div> <Header /> <div className=Styles.body> ...children </div> </div>,
};