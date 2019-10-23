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

  let textArea =
    Css.(
      style(
        [
          padding2(~v=`em(0.8), ~h=`em(1.4)),
          background(Colors.lightGrayBackground),
          borderRadius(Colors.BorderRadius.tight),
          color(Colors.Text.LightBackground.p),
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
          paddingBottom(`em(0.4)),
          borderBottom(`px(1), `solid, Colors.accentBlueO8),
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
  let standardCellPadding =
    Css.(style([paddingTop(`em(0.7)), paddingBottom(`em(0.5))]));

  let style = flexAmount => Css.(style([flex(flexAmount)]));

  let component = ReasonReact.statelessComponent("TABLE CELL");

  let make = (~flex, ~className="", ~properties=[], children) => {
    ...component,
    render: _self =>
      <Div.Jsx2
        className={Css.merge([
          style(flex),
          standardCellPadding,
          className,
          Css.style(properties),
        ])}>
        ...children
      </Div.Jsx2>,
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
            padding2(~v=`em(0.1), ~h=defaultRowHorizontalPadding),
          ]
          @ BaseStyles.fullWidthFloatLeft,
        )
      );
  };

  [@react.component]
  let make = (~children) =>
    <Div.Jsx2 styles=[Styles.headerRow]> ...children </Div.Jsx2>;

  module Jsx2 = {
    let component = ReasonReact.statelessComponent(__MODULE__ ++ "Jsx2");

    let make = children =>
      ReasonReactCompat.wrapReactForReasonReact(
        make,
        makeProps(~children, ()),
        children,
      );
  };
};

module Row = {
  let component = ReasonReact.statelessComponent("TABLE ROW");

  let textSection = text =>
    <Div.Jsx2 styles=[Styles.textArea]> text </Div.Jsx2>;

  let make = (~className="", ~bottomSubRow=?, ~onClick=?, children) => {
    ...component,
    render: _self => {
      let commonClasses =
        onClick |> E.O.isSome
          ? [Styles.clickableRow, className] : [className];
      switch (bottomSubRow) {
      | Some(bottomSubRow) =>
        <Div.Jsx2 styles=commonClasses ?onClick>
          <Div.Jsx2 styles=[Styles.topRow]> ...children </Div.Jsx2>
          <Div.Jsx2 styles=[Styles.bottomRow]> ...bottomSubRow </Div.Jsx2>
        </Div.Jsx2>
      | None =>
        <Div.Jsx2 styles=[Styles.row, ...commonClasses] ?onClick>
          ...children
        </Div.Jsx2>
      };
    },
  };
};

let component = ReasonReact.statelessComponent("Table");
let make = children => {
  ...component,
  render: _self => <div> ...children </div>,
};
