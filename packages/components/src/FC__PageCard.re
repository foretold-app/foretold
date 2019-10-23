open FC__Base;

let link =
  Link.Jsx2.make(
    ~className=
      Css.(
        style([
          marginRight(`em(2.)),
          color(Colors.accentBlue),
          hover([color(Colors.darkAccentBlue)]),
        ])
      ),
  );

[@react.component]
let make = (~children) =>
  <Div.Jsx2
    styles=[
      Css.(
        style(
          [
            background(Colors.white),
            border(`px(1), `solid, Colors.border),
            borderRadius(`px(5)),
          ]
          @ BaseStyles.fullWidthFloatLeft,
        )
      ),
    ]>
    ...children
  </Div.Jsx2>;

module Jsx2 = {
  let make = children =>
    ReasonReactCompat.wrapReactForReasonReact(
      make,
      makeProps(~children, ()),
      children,
    );
};

let defaultPadding = Css.padding2(~v=`em(0.0), ~h=`em(1.5));

module HeaderRow = {
  module Styles = {
    let itemTopPadding = Css.paddingTop(`em(0.5));
    let itemBottomPadding = Css.paddingBottom(`em(0.35));
    let itemRightPadding = Css.paddingRight(`em(0.9));
  };

  module Title = {
    [@react.component]
    let make = (~children) =>
      <Div.Jsx2
        styles=[
          Css.(
            style([
              color(Colors.textDark),
              paddingTop(`em(0.6)),
              paddingBottom(`em(0.6)),
              FC__Settings.FontWeights.heavy,
            ])
          ),
        ]>
        ...children
      </Div.Jsx2>;

    module Jsx2 = {
      let make = children =>
        ReasonReactCompat.wrapReactForReasonReact(
          make,
          makeProps(~children, ()),
          children,
        );
    };
  };

  [@react.component]
  let make = (~children) =>
    <Div.Jsx2
      styles=[
        Css.(
          style(
            [
              borderBottom(`px(1), `solid, Colors.accentBlueO8),
              defaultPadding,
            ]
            @ BaseStyles.fullWidthFloatLeft,
          )
        ),
      ]>
      ...children
    </Div.Jsx2>;

  module Jsx2 = {
    let make = children =>
      ReasonReactCompat.wrapReactForReasonReact(
        make,
        makeProps(~children, ()),
        children,
      );
  };
};

module Body = {
  [@react.component]
  let make = (~children) =>
    <Div.Jsx2 styles=[Css.style(BaseStyles.fullWidthFloatLeft)]>
      ...children
    </Div.Jsx2>;

  module Jsx2 = {
    let make = children =>
      ReasonReactCompat.wrapReactForReasonReact(
        make,
        makeProps(~children, ()),
        children,
      );
  };
};

module BodyPadding = {
  [@react.component]
  let make = (~v=`em(1.5), ~children) =>
    <Div.Jsx2
      styles=[
        Css.style(
          [Css.padding2(~v, ~h=`em(1.5))] @ BaseStyles.fullWidthFloatLeft,
        ),
      ]>
      ...children
    </Div.Jsx2>;

  module Jsx2 = {
    let make = (~v=`em(1.5), children) =>
      ReasonReactCompat.wrapReactForReasonReact(
        make,
        makeProps(~v, ~children, ()),
        children,
      );
  };
};

module Section = {
  module StyleProps = {
    open Css;
    type background = [ | `white | `grey];
    type padding = [ | `all | `none | `top | `bottom];
    type border = [ | `none | `top | `bottom | `topBottom];

    let baseClass = style([clear(`both)]);
    let bgGrey = style([backgroundColor(FC__Settings.smokeWhite)]);
    let paddingAll = style([padding(`em(1.5))]);
    let paddingTop =
      style([
        padding4(~top=`em(1.5), ~right=`zero, ~bottom=`zero, ~left=`zero),
      ]);
    let paddingBottom =
      style([
        padding4(~top=`zero, ~right=`zero, ~bottom=`em(1.5), ~left=`zero),
      ]);
    let borderTop =
      style([borderTop(`px(1), `solid, FC__Settings.accentBlue1a)]);
    let borderBottom =
      style([borderBottom(`px(1), `solid, FC__Settings.accentBlue1a)]);
    let flexCls = style([display(`flex)]);

    [@bs.send] external push: (Js.Array.t('a), 'a) => unit = "";
    let toClasses =
        (background: background, border: border, padding: padding, flex: bool) => {
      // Array for more speed and some imperative ease
      let cls = [|baseClass|];
      // Background
      switch (background) {
      | `grey => push(cls, bgGrey)
      | _ => ()
      };
      // Padding
      switch (padding) {
      | `all => push(cls, paddingAll)
      | `none => ()
      | `top => push(cls, paddingTop)
      | `bottom => push(cls, paddingBottom)
      };
      // Border
      switch (border) {
      | `none => ()
      | `top => push(cls, borderTop)
      | `bottom => push(cls, borderBottom)
      | `topBottom =>
        push(cls, borderTop);
        push(cls, borderBottom);
      };
      // Flex
      if (flex) {
        push(cls, flexCls);
      };
      Js.Array.joinWith(" ", cls);
    };
  };

  /**
   * Section of a PageCard
   * background: `white (default) | `grey
   * border: `top | `bottom | `none (default)
   * padding: `none | `top | `bottom | `all (default)
   * flex: true | false
   */
  [@react.component]
  let make =
      (
        ~background: StyleProps.background=`white,
        ~border: StyleProps.border=`none,
        ~padding: StyleProps.padding=`all,
        ~flex=false,
        ~children,
      ) =>
    <div className={StyleProps.toClasses(background, border, padding, flex)}>
      ...children
    </div>;

  module Jsx2 = {
    let make =
        (
          ~background: StyleProps.background=`white,
          ~border: StyleProps.border=`none,
          ~padding: StyleProps.padding=`all,
          ~flex=false,
          children,
        ) =>
      ReasonReactCompat.wrapReactForReasonReact(
        make,
        makeProps(~background, ~border, ~padding, ~flex, ~children, ()),
        children,
      );
  };
};

module VerticalSpace = {
  let component =
    ReasonReact.statelessComponent("Card Vertical Padding Area");
  let spaceStyle = Css.(style([marginTop(`em(1.5))]));
  let make = _children => {
    ...component,
    render: _self => <div className=spaceStyle />,
  };
};

module H1 = {
  let component = ReasonReact.statelessComponent("H1");
  let make = children => {
    ...component,
    render: _self =>
      <h1
        className=Css.(
          style(
            [
              fontSize(`em(1.15)),
              color(`hex("192D44")),
              FC__Settings.FontWeights.heavy,
              marginTop(`em(0.0)),
              marginBottom(`em(0.4)),
            ]
            @ BaseStyles.fullWidthFloatLeft,
          )
        )>
        ...children
      </h1>,
  };
};

module P = {
  let component = ReasonReact.statelessComponent("P");
  let make = children => {
    ...component,
    render: _self =>
      <p
        className=Css.(
          style([
            color(Colors.Text.LightBackground.p),
            lineHeight(`em(1.5)),
          ])
        )>
        ...children
      </p>,
  };
};
