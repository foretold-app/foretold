// Generated by BUCKLESCRIPT VERSION 4.0.5, PLEASE EDIT WITH CARE
'use strict';

var Css = require("bs-css/src/Css.js");
var Block = require("bs-platform/lib/js/block.js");
var React = require("react");
var Antd_Grid = require("bs-ant-design/src/Antd_Grid.js");
var Antd_Menu = require("bs-ant-design/src/Antd_Menu.js");
var Antd_Layout = require("bs-ant-design/src/Antd_Layout.js");
var ReasonReact = require("reason-react/src/ReasonReact.js");

function ste(prim) {
  return prim;
}

var component = ReasonReact.statelessComponent("User");

function link(url, name) {
  return React.createElement("a", {
              onClick: (function () {
                  return ReasonReact.Router[/* push */0](url);
                })
            }, name);
}

var header = Css.style(/* :: */[
      Css.width(/* `percent */[
            -119887163,
            100.0
          ]),
      /* :: */[
        Css.backgroundColor(Css.white),
        /* :: */[
          Css.selector(" h4", /* :: */[
                Css.marginTop(Css.px(2)),
                /* [] */0
              ]),
          /* [] */0
        ]
      ]
    ]);

var menu = Css.style(/* :: */[
      Css.lineHeight(Css.px(64)),
      /* [] */0
    ]);

var Styles = /* module */[
  /* header */header,
  /* menu */menu
];

Css.$$global("body", /* :: */[
      Css.fontFamily("Lato"),
      /* [] */0
    ]);

function make() {
  return /* record */[
          /* debugName */component[/* debugName */0],
          /* reactClassInternal */component[/* reactClassInternal */1],
          /* handedOffState */component[/* handedOffState */2],
          /* willReceiveProps */component[/* willReceiveProps */3],
          /* didMount */component[/* didMount */4],
          /* didUpdate */component[/* didUpdate */5],
          /* willUnmount */component[/* willUnmount */6],
          /* willUpdate */component[/* willUpdate */7],
          /* shouldUpdate */component[/* shouldUpdate */8],
          /* render */(function () {
              return ReasonReact.element(undefined, undefined, Antd_Grid.Row[/* make */8](undefined, /* ResponsiveBreakpoints */Block.__(1, [Antd_Grid.makeGutterBreakpoints(undefined, 5, undefined, undefined, undefined, undefined, /* () */0)]), undefined, undefined, undefined, undefined, undefined, /* array */[ReasonReact.element(undefined, undefined, Antd_Grid.Col[/* make */1](undefined, 24, undefined, undefined, undefined, undefined, undefined, undefined, undefined, undefined, undefined, undefined, undefined, undefined, /* array */[ReasonReact.element(undefined, undefined, Antd_Layout.Header[/* make */0](undefined, header, undefined, /* array */[ReasonReact.element(undefined, undefined, Antd_Menu.make(/* Light */136970422, /* Horizontal */208994564, undefined, undefined, undefined, undefined, undefined, undefined, undefined, undefined, undefined, undefined, undefined, menu, undefined, undefined, undefined, undefined, undefined, undefined, /* array */[
                                                            ReasonReact.element(undefined, undefined, Antd_Menu.Item[/* make */0](undefined, undefined, undefined, undefined, undefined, /* array */[link("/users", "Users")])),
                                                            ReasonReact.element(undefined, undefined, Antd_Menu.Item[/* make */0](undefined, undefined, undefined, undefined, undefined, /* array */[link("/measurables", "Measurables")]))
                                                          ]))]))]))]));
            }),
          /* initialState */component[/* initialState */10],
          /* retainedProps */component[/* retainedProps */11],
          /* reducer */component[/* reducer */12],
          /* jsElementWrapped */component[/* jsElementWrapped */13]
        ];
}

exports.ste = ste;
exports.component = component;
exports.link = link;
exports.Styles = Styles;
exports.make = make;
/* component Not a pure module */
