module BaseStyles = {
  let floatLeft = Css.float(`left);
  let floatRight = Css.float(`right);
  let fullWidthFloatLeft =
    Css.[floatLeft, width(`percent(100.0)), boxSizing(`borderBox)];

  let borderNone = Css.[borderBottom(`px(0), `solid, hex("fff"))];
};

module Types = FC__Types;
module Link = FC__Link;
module Colors = FC__Colors;
module Div = FC__Div;
module E = FC__E;