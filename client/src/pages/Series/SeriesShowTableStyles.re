open Css;

let row = m =>
  style([
    width(`percent(100.0)),
    borderBottom(`px(1), `solid, hex("eee")),
    display(`flex),
    flexDirection(`row),
    paddingLeft(px(8)),
    paddingRight(px(8)),
    paddingTop(px(8)),
    paddingBottom(px(7)),
    cursor(`pointer),
    selector(":last-child", [borderBottom(`px(0), `solid, hex("eee"))]),
    selector(":hover", [backgroundColor(`hex("eef0f3"))]),
  ]);

let column = style([flex(1)]);