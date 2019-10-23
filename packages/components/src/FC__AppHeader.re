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

let link =
  Link.Jsx2.make(
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

[@react.component]
let make = (~links: ReasonReact.reactElement) =>
  <Div.Jsx2 styles=[Styles.outer]>
    <Div.Jsx2 float=`left> links </Div.Jsx2>
  </Div.Jsx2>;

module Jsx2 = {
  let component = ReasonReact.statelessComponent(__MODULE__ ++ "Jsx2");

  let make = (~links: ReasonReact.reactElement, children) =>
    ReasonReactCompat.wrapReactForReasonReact(
      make,
      makeProps(~links, ()),
      children,
    );
};
