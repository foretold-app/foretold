module Cell = {
  let style = flexAmount => Css.(style([flex(flexAmount)]));

  let component = ReasonReact.statelessComponent("TABLE CELL");

  let make = (~flex, ~styles, children) => {
    ...component,
    render: _self =>
      <Div styles={[style(flex)] @ styles}> ...children </Div>,
  };
};

module HeaderRow = {
  let defaultPadding = Css.padding2(~v=`em(0.0), ~h=`em(1.5));
  let style =
    Css.(
      style(
        [
          background(`hex("e7eaef")),
          color(Colors.Text.LightBackground.main),
          borderBottom(`px(1), `solid, Colors.accentBlueO8),
          display(`flex),
          flexDirection(`row),
          defaultPadding,
        ]
        @ Base.fullWidthFloatLeft,
      )
    );

  let component = ReasonReact.statelessComponent("TABLE HEADER ROW");

  let make = children => {
    ...component,
    render: _self => <Div styles=[style]> ...children </Div>,
  };
};

module Row = {
  let style =
    Css.(
      style(
        [
          padding2(~v=`px(0), ~h=`em(1.5)),
          borderBottom(`px(1), `solid, Colors.accentBlueO8),
          display(`flex),
          flexDirection(`row),
        ]
        @ Base.fullWidthFloatLeft,
      )
    );

  let headerStyle =
    Css.style([
      Css.fontSize(`em(1.2)),
      Css.lineHeight(`em(1.2)),
      Css.color(Colors.black),
      Css.fontWeight(`num(500)),
    ]);

  let component = ReasonReact.statelessComponent("TABLE ROW");

  let make = children => {
    ...component,
    render: _self => <Div styles=[style]> ...children </Div>,
  };
};