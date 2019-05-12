/* // this is used to show hex color; */
let removeHex = Js.String.sliceToEnd(~from=1);
let r = c => c->removeHex->(e => `hex(e));

let white = "#FFFFFF"->r;
let black = "#000000"->r;
let greyO4 = "#00000044"->r;
let clear = "#00000000"->r;

let textDark = "#192D44"->r;
let smokeWhite = "#F0F1F3"->r;
let greydisabled = "#e3e4e6"->r;
let accentBlue = "#8C9EB5"->r;
let mainBlue = "#347296"->r;
let darkAccentBlue = "#5C6E95"->r;
let grey1 = "#868686"->r;

module Statuses = {
  let green = "#6C8A64"->r;
  let red = "#925151"->r;
  let yellow = "#C09C66"->r;
};

let primary = mainBlue;

module Text = {
  module LightBackground = {
    let main = textDark;
    let light = accentBlue;
    let active = "#3562AE"->r;
  };
};