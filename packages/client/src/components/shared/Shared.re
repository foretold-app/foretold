open Css;
module Item = {
  let id =
    style([
      Css.float(`left),
      marginTop(`em(0.3)),
      color(`hex("b2b2b2")),
      fontSize(`em(0.8)),
    ]);

  let item =
    style([
      Css.float(`left),
      fontSize(`em(1.0)),
      marginRight(`px(18)),
      color(`hex("bbb")),
      selector(
        " a",
        [color(`hex("aaa")), selector(":hover", [color(hex("333"))])],
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
      Css.float(`left),
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
    fontSize(`em(1.0)),
    textDecoration(`underline),
    fontWeight(`num(600)),
  ];

  let primarylinkColors = [
    color(`hex("0e2b68")),
    selector(":hover", [color(`hex("666"))]),
    ..._linkCommonAttributes,
  ];

  let item = style([marginRight(`em(0.4)), ...primarylinkColors]);

  let property = style([marginRight(`em(0.1)), ...primarylinkColors]);

  let largeItem = style([marginRight(`px(8)), ...primarylinkColors]);

  let dateItem =
    style([
      color(`hex("666")),
      important(textDecorationColor(`hex("ccc"))),
      ..._linkCommonAttributes,
    ]);

  let conditionalOn =
    style([
      color(`hex("666")),
      important(textDecorationColor(`hex("ccc"))),
      selector(
        "span:not(:empty):not(:last-child):after",
        [contentRule(", ")],
      ),
      ..._linkCommonAttributes,
    ]);
};