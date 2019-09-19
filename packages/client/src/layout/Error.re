open Utils;

module Styles = {
  open Css;
  let centerBlock = style([textAlign(`center), padding(`em(2.))]);
  let clear = style([clear(`both)]);
};

let component = ReasonReact.statelessComponent("Error");

let make = (~e, _children) => {
  ...component,
  render: _ => <> <div> {"Error: " ++ e |> Utils.ste} </div> </>,
};