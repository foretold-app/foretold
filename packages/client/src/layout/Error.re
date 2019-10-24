[@bs.config {jsx: 3}];

open Utils;

module Styles = {
  open Css;
  let centerBlock = style([textAlign(`center), padding(`em(2.))]);
  let clear = style([clear(`both)]);
};

[@react.component]
let make = (~e) => <> <div> {"Error: " ++ e |> Utils.ste} </div> </>;
