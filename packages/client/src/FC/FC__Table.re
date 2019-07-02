open FC__Base;

let defaultRowHorizontalPadding = `em(1.5);

module Styles = {
  let row =
    Css.(
      style(
        [
          padding2(~v=`zero, ~h=defaultRowHorizontalPadding),
          borderBottom(`px(1), `solid, Colors.accentBlue1a),
          display(`flex),
          flexDirection(`row),
          selector(":last-child", BaseStyles.borderNone),
        ]
        @ BaseStyles.fullWidthFloatLeft,
      )
    );

  let topRow =
    Css.(
      style(
        [
          padding2(~v=`zero, ~h=defaultRowHorizontalPadding),
          paddingTop(`em(0.4)),
          display(`flex),
          flexDirection(`row),
        ]
        @ BaseStyles.fullWidthFloatLeft,
      )
    );

  let bottomRow =
    Css.(
      style(
        [
          padding2(~v=`zero, ~h=`em(0.4)),
          paddingBottom(`em(0.3)),
          borderBottom(`px(1), `solid, Colors.accentBlueO8),
          display(`flex),
          flexDirection(`row),
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

  module Elements = {
    let primaryText =
      Css.style([
        Css.fontSize(`em(1.05)),
        Css.lineHeight(`em(1.5)),
        Css.fontWeight(`num(600)),
        Css.color(`hex("384e67")),
      ]);

    let link' =
      Css.[
        marginRight(`em(1.0)),
        color(Colors.textMedium),
        hover([color(Colors.textDark)]),
      ];

    let link = (~isUnderlined=false, ()) =>
      Css.(
        style(isUnderlined ? link' @ [textDecoration(`underline)] : link')
      );
  };
};

module Cell = {
  let style = flexAmount => Css.(style([flex(flexAmount)]));

  let component = ReasonReact.statelessComponent("TABLE CELL");

  let make = (~flex, ~className="", children) => {
    ...component,
    render: _self =>
      <Div className={Css.merge([style(flex), className])}>
        ...children
      </Div>,
  };
};

module HeaderRow = {
  let component = ReasonReact.statelessComponent("TABLE HEADER ROW");
  module Styles = {
    let headerRow =
      Css.(
        style(
          [
            background(Colors.lightGrayBackground),
            color(Colors.Text.LightBackground.main),
            borderBottom(`px(1), `solid, Colors.accentBlueO8),
            display(`flex),
            flexDirection(`row),
            padding2(~v=`em(0.7), ~h=defaultRowHorizontalPadding),
          ]
          @ BaseStyles.fullWidthFloatLeft,
        )
      );
  };

  let make = children => {
    ...component,
    render: _self => <Div styles=[Styles.headerRow]> ...children </Div>,
  };
};

module Row = {
  let component = ReasonReact.statelessComponent("TABLE ROW");

  let textSection = text => <FC__Quote> text </FC__Quote>;

  let make = (~className="", ~bottomSubRow=?, ~onClick=?, children) => {
    ...component,
    render: _self => {
      let commonClasses =
        onClick |> E.O.isSome
          ? [Styles.clickableRow, className] : [className];
      switch (bottomSubRow) {
      | Some(bottomSubRow) =>
        <Div styles=commonClasses ?onClick>
          <Div styles=[Styles.topRow]> ...children </Div>
          <Div styles=[Styles.bottomRow]> ...bottomSubRow </Div>
        </Div>
      | None =>
        <Div styles=[Styles.row, ...commonClasses] ?onClick>
          ...children
        </Div>
      };
    },
  };
};