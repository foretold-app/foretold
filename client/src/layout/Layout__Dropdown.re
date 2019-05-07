module Styles = {
  open Css;
  let dropdown =
    style([
      border(`px(1), `solid, `hex("d5d2d2")),
      padding2(~v=`em(0.5), ~h=`em(0.)),
      borderRadius(`px(5)),
      background(`hex("fff")),
      boxShadow(
        ~x=`px(1),
        ~y=`px(1),
        ~blur=`px(5),
        ~spread=`px(1),
        ~inset=false,
        `hex("dfd7d7"),
      ),
    ]);
  let actions = [float(`left), maxWidth(`em(17.))] |> style;
  let action =
    style([
      cursor(`pointer),
      padding2(~v=`em(0.3), ~h=`em(1.4)),
      minWidth(`px(200)),
      fontSize(`em(1.2)),
      width(`percent(100.)),
      float(`left),
      color(`hex("666")),
      selector(
        ":hover",
        [color(`hex("666")), backgroundColor(`hex("eef0f3"))],
      ),
    ]);
};