open Css;
/* TODO: Delete me! */
let group =
  style([
    border(`px(1), `solid, hex("eee")),
    backgroundColor(hex("fafafa")),
  ]);

let row = m => {
  let statusOpacity = (measurable: DataModel.Measurable.t) => {
    let state = measurable.state |> E.O.toExn("Needs state from GraphQL");
    if (state === `ARCHIVED) {
      0.8;
    } else if (state === `JUDGED) {
      0.55;
    } else {
      1.0;
    };
  };
  style([
    width(`percent(100.0)),
    borderBottom(`px(1), `solid, hex("eee")),
    selector(" h2", [marginTop(px(2))]),
    display(`flex),
    opacity(statusOpacity(m)),
    flexDirection(`row),
    paddingLeft(px(8)),
    paddingRight(px(8)),
    paddingTop(px(8)),
    paddingBottom(px(7)),
    cursor(`pointer),
    selector(":last-child", [borderBottom(`px(0), `solid, hex("eee"))]),
    selector(":hover", [backgroundColor(`hex("eef0f3"))]),
  ]);
};

let mainColumn = style([flex(5), display(`flex), flexDirection(`column)]);

let mainColumnTop =
  style([
    flex(1),
    paddingLeft(px(2)),
    selector(" p", [marginTop(`px(3)), marginBottom(`px(8))]),
  ]);

let mainColumnBottom =
  style([flex(1), padding(`px(2)), marginTop(`px(2))]);
let rightColumn = style([flex(1)]);

let linkS = [
  padding2(~v=`px(1), ~h=`px(4)),
  borderRadius(`px(2)),
  fontSize(`em(0.95)),
  fontWeight(`num(800)),
];

let itemLink =
  style([
    color(`hex("0e2b68")),
    backgroundColor(`hex("dbdbe5")),
    selector(
      ":hover",
      [backgroundColor(`hex("c7defe")), color(`hex("0e2b68"))],
    ),
    marginRight(`em(0.4)),
    ...linkS,
  ]);

let propertyLink =
  style([
    color(`hex("0e2b68")),
    backgroundColor(`hex("dbdbe5")),
    selector(
      ":hover",
      [backgroundColor(`hex("c7defe")), color(`hex("0e2b68"))],
    ),
    marginRight(`em(0.1)),
    ...linkS,
  ]);

let largeItemLink =
  style([
    color(`hex("0e2b68")),
    backgroundColor(`hex("dbdbe5")),
    selector(
      ":hover",
      [backgroundColor(`hex("c7defe")), color(`hex("0e2b68"))],
    ),
    padding2(~v=`px(1), ~h=`px(4)),
    borderRadius(`px(2)),
    marginRight(`px(8)),
    fontSize(`px(16)),
    fontWeight(`num(800)),
  ]);

let calDate =
  style([
    marginLeft(`em(0.3)),
    lineHeight(`em(1.56)),
    fontSize(`em(1.1)),
  ]);

let namme = style([fontSize(`em(1.2)), color(`hex("333"))]);

let calDateO =
  style([
    color(`hex("505050")),
    backgroundColor(`hex("ececec")),
    marginRight(`px(2)),
    ...linkS,
  ]);

let bigLink = style([fontSize(`px(24))]);
let mainLink = style([fontSize(`px(14))]);

let date =
  style([
    borderRadius(`px(2)),
    padding2(~v=`px(0), ~h=`px(5)),
    color(`hex("333")),
    fontSize(`px(18)),
    selector(
      ":hover",
      [backgroundColor(`hex("ddd")), color(`hex("333"))],
    ),
  ]);

let creatorLinkLeftMargin =
  style([float(`left), marginLeft(`px(5)), height(`percent(100.0))]);