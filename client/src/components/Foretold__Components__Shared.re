open Css;
module Item = {
  let item =
    style([
      float(`left),
      fontSize(`px(12)),
      marginRight(`px(18)),
      color(`hex("bbb")),
      selector(
        " a",
        [color(`hex("777")), selector(":hover", [color(hex("333"))])],
      ),
    ]);

  type buttonStyle =
    | NORMAL
    | DANGER;

  type buttonStyleColors = {
    background: string,
    backgroundHovered: string,
    color: string,
    colorHovered: string,
  };

  let buttonColors = (t: buttonStyle) =>
    switch (t) {
    | NORMAL => {
        background: "ece7e7",
        backgroundHovered: "cec8c8",
        color: "908a8a",
        colorHovered: "423636",
      }
    | DANGER => {
        background: "eadede",
        backgroundHovered: "f9c8c8",
        color: "a08181",
        colorHovered: "671919",
      }
    };

  let itemButton = (t: buttonStyle) => {
    let colors = buttonColors(t);
    style([
      color(`hex(colors.color)),
      borderRadius(`px(2)),
      padding2(~v=`px(1), ~h=`px(4)),
      Css.cursor(`pointer),
      float(`left),
      backgroundColor(hex(colors.background)),
      selector(
        ":hover",
        [
          backgroundColor(hex(colors.backgroundHovered)),
          color(`hex(colors.colorHovered)),
        ],
      ),
    ]);
  };
};