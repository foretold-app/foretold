module Styles = {
  open Css;
  let group =
    style([
      border(`px(1), `solid, hex("eee")),
      backgroundColor(hex("fafafa")),
    ]);
  let row =
    style([
      width(`percent(100.0)),
      borderBottom(`px(1), `solid, hex("eee")),
      selector(" h2", [marginTop(px(2))]),
      display(`flex),
      flexDirection(`row),
      paddingLeft(px(5)),
      paddingRight(px(5)),
      paddingTop(px(5)),
      paddingBottom(px(5)),
    ]);

  let mainColumn =
    style([
      flex(5),
      display(`flex),
      flexDirection(`column),
      fontSize(px(12)),
    ]);
  let item =
    style([
      background(hex("e6e6e6")),
      float(`left),
      marginRight(`px(4)),
      padding2(~v=`px(0), ~h=`px(1)),
      borderRadius(`px(1)),
    ]);
  let mainColumnTop =
    style([
      flex(1),
      paddingLeft(px(2)),
      paddingBottom(px(4)),
      selector(
        " a",
        [
          color(`hex("666")),
          fontSize(px(16)),
          selector(":hover", [backgroundColor(hex("ddd"))]),
        ],
      ),
    ]);
  let mainColumnBottom = style([flex(1), padding(px(2))]);
  let rightColumn = style([flex(1)]);
  let statusRow =
    style([
      flex(1),
      selector(
        " h3",
        [
          marginTop(px(2)),
          marginBottom(px(0)),
          fontSize(px(14)),
          color(`hex("666")),
        ],
      ),
      selector(" p", [marginBottom(px(2)), fontSize(px(12))]),
    ]);
};