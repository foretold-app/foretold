// Generated by BUCKLESCRIPT VERSION 4.0.5, PLEASE EDIT WITH CARE
'use strict';

var $$Array = require("bs-platform/lib/js/array.js");
var Curry = require("bs-platform/lib/js/curry.js");
var React = require("react");
var ReasonReact = require("reason-react/src/ReasonReact.js");
var Utils$Client = require("../utils/Utils.bs.js");
var Queries$Client = require("./Queries.bs.js");
var Result$Rationale = require("rationale/src/Result.js");
var NormalLayout$Client = require("../utils/NormalLayout.bs.js");

function showQueryList(data, urlFn, render) {
  return $$Array.mapi((function (index, element) {
                return React.createElement("div", {
                            key: String(index),
                            onClick: (function () {
                                return ReasonReact.Router[/* push */0](Curry._1(urlFn, element));
                              })
                          }, Curry._1(render, element));
              }), data);
}

function arg(user) {
  return "/users/" + user.id;
}

function arg$1(e) {
  return Utils$Client.ste(e.name);
}

function userList(param) {
  return showQueryList(param, arg, arg$1);
}

var component = ReasonReact.statelessComponent("Users");

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
              var eta = Curry.app(Queries$Client.GetUsersQuery[/* make */3], [
                    undefined,
                    undefined,
                    undefined,
                    undefined,
                    undefined,
                    undefined,
                    undefined,
                    undefined,
                    undefined,
                    (function (o) {
                        return Result$Rationale.result(Utils$Client.idd, Utils$Client.idd, Curry._2(Result$Rationale.Infix[/* <$> */1], Curry._2(Result$Rationale.Infix[/* <$> */1], Curry._2(Result$Rationale.Infix[/* <$> */1], Utils$Client.apolloResponseToResult(o[/* result */0]), (function (d) {
                                                  return d.users;
                                                })), Utils$Client.catOptionals), userList));
                      })
                  ]);
              var eta$1 = NormalLayout$Client.make("Users Page", ReasonReact.element(undefined, undefined, eta));
              return ReasonReact.element(undefined, undefined, eta$1);
            }),
          /* initialState */component[/* initialState */10],
          /* retainedProps */component[/* retainedProps */11],
          /* reducer */component[/* reducer */12],
          /* jsElementWrapped */component[/* jsElementWrapped */13]
        ];
}

exports.showQueryList = showQueryList;
exports.userList = userList;
exports.component = component;
exports.make = make;
/* component Not a pure module */
