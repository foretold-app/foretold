module Styles = {
  open Css;
  let outer =
    style(
      [
        padding2(~v=`em(1.0), ~h=`em(2.)),
        backgroundColor(`rgb((255, 255, 255))),
      ]
      @ Base.fullWidthFloatLeft,
    );
};

let component = ReasonReact.statelessComponent(__MODULE__);

let link =
  BaseLink.make(
    ~colors=(
      Colors.Text.LightBackground.main,
      Colors.Text.LightBackground.main,
    ),
    ~isDisabled=false,
    ~styles=Css.[marginRight(`em(2.)), fontWeight(`num(400))],
  );

let make = (~links: ReasonReact.reactElement, _children) => {
  ...component,
  render: _self =>
    <Div styles=[Styles.outer]> <Div float=`left> links </Div> </Div>,
};