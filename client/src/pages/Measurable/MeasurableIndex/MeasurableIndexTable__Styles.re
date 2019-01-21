module Styles = {
  open Css;
  let row =
    style([
      width(`percent(100.0)),
      backgroundColor(hex("f6f6f6")),
      marginBottom(px(2)),
      selector(" h2", [marginTop(px(2))]),
      display(`flex),
      flexDirection(`row),
      paddingLeft(px(5)),
      paddingRight(px(5)),
    ]);

  let nameRow =
    style([
      flex(4),
      padding(px(2)),
      selector(
        " a",
        [
          color(`hex("666")),
          fontSize(px(14)),
          selector(":hover", [backgroundColor(hex("ddd"))]),
        ],
      ),
    ]);
  let ownerRow = style([flex(1), padding(px(2))]);
  let statsRow = style([flex(1), padding(px(2))]);
  let columnHeader = style([color(hex("222"))]);
  let statusRow =
    style([
      flex(1),
      padding(px(2)),
      selector(
        " h3",
        [marginTop(px(2)), marginBottom(px(0)), fontSize(px(14))],
      ),
      selector(" p", [marginBottom(px(2)), fontSize(px(12))]),
    ]);
  let menu = style([lineHeight(px(64))]);
};