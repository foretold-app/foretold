let style =
  Css.(
    style(
      [
        padding2(~v=`px(0), ~h=`em(1.5)),
        borderBottom(`px(1), `solid, `hex("D5D7DAAA")),
      ]
      @ Base.fullWidthFloatLeft,
    )
  );

let headerStyle =
  Css.style([Css.fontSize(`em(1.2)), Css.color(Colors.black)]);

let component = ReasonReact.statelessComponent(__MODULE__);

let make = children => {
  ...component,
  render: _self => <Div styles=[style]> ...children </Div>,
};