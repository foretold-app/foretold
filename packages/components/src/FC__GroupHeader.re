open FC__Base;

module Colors' = {
  let background = Colors.mainBlue;
  let primary = Colors.white;
  let secondary = `hex("ffffffb1");
};

module Styles = {
  open Css;
  let outer =
    style(
      [backgroundColor(Colors'.background)] @ BaseStyles.fullWidthFloatLeft,
    );

  let inner =
    style(
      [boxSizing(`borderBox), padding2(~v=`em(1.), ~h=`em(2.0))]
      @ BaseStyles.fullWidthFloatLeft,
    );

  let hero =
    style([
      fontSize(`px(24)),
      color(Colors'.primary),
      fontFamily("poppins"),
    ]);
};

let linkStyles =
  Css.[
    BaseStyles.floatRight,
    marginLeft(`em(2.)),
    marginTop(`em(0.5)),
    borderRadius(`px(5)),
    padding2(~v=`em(0.45), ~h=`em(2.1)),
  ];

let link =
  Link.make(
    ~colors=(`hex("3562AE"), Colors.darkAccentBlue),
    ~isDisabled=false,
    ~styles=
      linkStyles
      @ Css.[
          background(Colors'.primary),
          Css.transition(~duration=100, "background"),
          selector(
            ":hover",
            [background(Colors.whiteOc), color(Colors.darkAccentBlue)],
          ),
        ],
  );

let secondaryLink =
  Link.make(
    ~colors=(Colors'.secondary, Colors.darkAccentBlue),
    ~isDisabled=false,
    ~styles=
      linkStyles
      @ Css.[
          border(`px(1), `solid, Colors'.secondary),
          Css.transition(~duration=100, "background"),
          selector(":hover", [background(Colors.whiteO2)]),
        ],
  );

let component = ReasonReact.statelessComponent(__MODULE__);

let make = (~hero: ReasonReact.reactElement, children) => {
  ...component,
  render: _self =>
    <Div styles=[Styles.outer]>
      <Div styles=[Styles.inner]> ...children </Div>
    </Div>,
};

module SubHeader = {
  open Colors.Text;
  let component = ReasonReact.statelessComponent(__MODULE__ ++ "Subheader");

  let make = children => {
    ...component,
    render: _self =>
      <Div
        styles=[
          Css.(
            style(
              [backgroundColor(Colors.white)] @ BaseStyles.fullWidthFloatLeft,
            )
          ),
        ]>
        <Div
          styles=[
            Css.(
              style(
                [
                  padding2(~v=`em(0.0), ~h=`em(2.0)),
                  boxShadow(
                    ~x=`px(1),
                    ~y=`px(1),
                    ~blur=`px(4),
                    Colors.greyO4,
                  ),
                ]
                @ BaseStyles.fullWidthFloatLeft,
              )
            ),
          ]>
          ...children
        </Div>
      </Div>,
  };
};