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
      cursor(`pointer),
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

module TagLink = {
  let _linkCommonAttributes = [
    padding2(~v=`px(1), ~h=`px(4)),
    borderRadius(`px(2)),
    fontSize(`em(0.95)),
    fontWeight(`num(800)),
  ];

  let primarylinkColors = [
    color(`hex("0e2b68")),
    backgroundColor(`hex("dbdbe5")),
    selector(
      ":hover",
      [backgroundColor(`hex("c7defe")), color(`hex("0e2b68"))],
    ),
    ..._linkCommonAttributes,
  ];

  let item = style([marginRight(`em(0.4)), ...primarylinkColors]);

  let property = style([marginRight(`em(0.1)), ...primarylinkColors]);

  let largeItem = style([marginRight(`px(8)), ...primarylinkColors]);

  let dateItem =
    style([
      color(`hex("505050")),
      backgroundColor(`hex("ececec")),
      marginRight(`px(2)),
      ..._linkCommonAttributes,
    ]);
};