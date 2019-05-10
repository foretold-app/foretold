module Styles = {
  open Css;
  let outer =
    style([
      padding2(~v=`em(1.0), ~h=`em(2.)),
      float(`left),
      backgroundColor(`rgb((255, 255, 255))),
      boxSizing(`borderBox),
      width(`percent(100.)),
    ]);
};

let component = ReasonReact.statelessComponent(__MODULE__);

let link =
  Main.BaseLink.make(
    ~colors=(Colors.accentBlue, Colors.darkAccentBlue),
    ~styles=Css.[marginRight(`em(2.)), fontWeight(`num(400))],
  );

let make = (~links: ReasonReact.reactElement, _children) => {
  ...component,
  render: _self =>
    <Div styles=[Styles.outer]> <Div float=`left> links </Div> </Div>,
};