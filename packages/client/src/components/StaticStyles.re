// @todo: To move these styles somewhere else.
open Css;
let body =
  style([
    backgroundImage(
      `linearGradient((
        `deg(60.),
        [
          (`percent(0.), `hex("1f6698")),
          (`percent(100.), `hex("1f9298")),
        ],
      )),
    ),
    textAlign(`center),
    height(`percent(100.)),
  ]);

let title =
  style([
    paddingTop(`em(3.)),
    fontSize(`em(3.)),
    color(`hex("fff")),
    opacity(0.8),
    fontWeight(`num(500)),
  ]);

let description =
  style([
    paddingTop(`em(1.)),
    fontSize(`em(1.)),
    color(`hex("fff")),
    opacity(0.8),
    selector("a", [color(`hex("fff")), textDecoration(`underline)]),
    fontWeight(`num(500)),
  ]);