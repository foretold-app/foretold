open Utils;
open Rationale.Result.Infix;
open Rationale;
open E;

let ste = ReasonReact.string;

let component = ReasonReact.statelessComponent("Home");

module Styles = {
  open Css;
  let body =
    style([
      backgroundImage(
        `linearGradient((
          `deg(60),
          [(0, `hex("1f6698")), (100, `hex("1f9298"))],
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
      fontWeight(`num(500)),
    ]);
};

let make = _children => {
  ...component,
  render: _ =>
    <div className=Styles.body>
      <div className=Styles.title> {"Foretold" |> ste} </div>
      <div className=Styles.description>
        {
          "Foretold is currently in development. Contact Ozzie for early access."
          |> ste
        }
      </div>
    </div>,
};