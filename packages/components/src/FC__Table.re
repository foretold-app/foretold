open FC__Base;

module Styles = {
  let defaultPadding = Css.padding2(~v=`em(0.0), ~h=`em(1.5));
  let headerRow =
    Css.(
      style(
        [
          background(Colors.lightGrayBackground),
          color(Colors.Text.LightBackground.main),
          borderBottom(`px(1), `solid, Colors.accentBlueO8),
          display(`flex),
          flexDirection(`row),
          defaultPadding,
        ]
        @ BaseStyles.fullWidthFloatLeft,
      )
    );

  let row =
    Css.(
      style(
        [
          padding2(~v=`px(0), ~h=`em(1.5)),
          borderBottom(`px(1), `solid, Colors.accentBlue1a),
          display(`flex),
          flexDirection(`row),
          selector(":last-child", BaseStyles.borderNone),
        ]
        @ BaseStyles.fullWidthFloatLeft,
      )
    );

  let clickableRow =
    Css.(
      style([
        hover([background(Colors.lightGrayBackground)]),
        cursor(`pointer),
      ])
    );
};

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
  let component = ReasonReact.statelessComponent("TABLE HEADER ROW");

  let make = children => {
    ...component,
    render: _self => <Div styles=[Styles.headerRow]> ...children </Div>,
  };
};

module Row = {
  let primaryText =
    Css.style([
      Css.fontSize(`em(1.05)),
      Css.lineHeight(`em(1.5)),
      Css.fontWeight(`num(600)),
      Css.color(`hex("384e67")),
    ]);

  let component = ReasonReact.statelessComponent("TABLE ROW");

  let make = children => {
    ...component,
    render: _self => <Div styles=[Styles.row]> ...children </Div>,
  };
};

module RowLink = {
  let component = ReasonReact.statelessComponent("TABLE ROW");

  let make = (~onClick, children) => {
    ...component,
    render: _self =>
      <Div styles=[Styles.row, Styles.clickableRow] onClick>
        ...children
      </Div>,
  };
};