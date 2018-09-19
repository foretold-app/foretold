let ste = ReasonReact.string;

let component = ReasonReact.statelessComponent("User");

let link = (url, name) =>
  <a onClick=(_e => ReasonReact.Router.push(url))>
    (name |> ReasonReact.string)
  </a>;

module Styles = {
  open Css;
  let header =
    style([
      background(hex("dcdcdc")),
      padding(px(5)),
      selector(" h4", [marginTop(px(2))]),
    ]);
};

Css.(global("body", [fontFamily("Lato")]));

let make = _children => {
  ...component,
  render: _ =>
    <div className=Styles.header>
      <h4> ("Prediction App" |> ste) </h4>
      (link("/users", "Users"))
      (link("/measurables", "Measurables"))
    </div>,
};