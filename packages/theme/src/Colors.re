/* // this is used to show hex color; */
let removeHex = Js.String.sliceToEnd(~from=1);
let r = c => c->removeHex->(e => `hex(e));

let white = "#FFFFFF"->r;
let smokeWhite = "#F0F1F3"->r;
let accentBlue = "#8C9EB5"->r;
let mainBlue = "#347296"->r;
let darkAccentBlue = "#5C6E95"->r;
let grey1 = "#868686"->r;