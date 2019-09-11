open FC__Base;

module Styles = {
  open Css;
  let outer =
    style(
      [
        padding2(~v=`em(1.0), ~h=`em(2.)),
        backgroundColor(`rgb((255, 255, 255))),
        position(`relative),
        boxShadow(
          ~x=`px(1),
          ~y=`px(1),
          ~blur=`px(2),
          ~spread=`px(1),
          ~inset=false,
          `hex("dee5e9"),
        ),
      ]
      @ BaseStyles.fullWidthFloatLeft,
    );
};

let component = ReasonReact.statelessComponent(__MODULE__);

let link =
  FC__Link.make(
    ~isDisabled=false,
    ~className=
      Css.(
        style([
          marginRight(`em(2.)),
          color(FC__Settings.Text.LightBackground.main),
          hover([color(FC__Settings.Text.LightBackground.main)]),
        ])
      ),
  );

let make = (~links: ReasonReact.reactElement, _children) => {
  ...component,
  render: _self =>
    <Div styles=[Styles.outer]> <Div float=`left> links </Div> </Div>,
};