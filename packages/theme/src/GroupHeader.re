module Colors' = {
  let background = Colors.mainBlue;
  let primary = Colors.white;
  let secondary = `hex("ffffffb1");
};

module Styles = {
  open Css;
  let outer =
    style([backgroundColor(Colors'.background)] @ Base.fullWidthFloatLeft);

  let inner =
    style(
      [boxSizing(`borderBox), padding2(~v=`em(1.), ~h=`em(2.0))]
      @ Base.fullWidthFloatLeft,
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
    float(`right),
    marginLeft(`em(2.)),
    marginTop(`em(0.5)),
    borderRadius(`px(5)),
    padding2(~v=`em(0.45), ~h=`em(2.1)),
  ];

let link =
  BaseLink.make(
    ~colors=(`hex("3562AE"), Colors.darkAccentBlue),
    ~isDisabled=false,
    ~styles=
      linkStyles
      @ Css.[
          background(Colors'.primary),
          selector(
            ":hover",
            [background(Colors.smokeWhite), color(Colors.darkAccentBlue)],
          ),
        ],
  );

let secondaryLink =
  BaseLink.make(
    ~colors=(Colors'.secondary, Colors.darkAccentBlue),
    ~isDisabled=false,
    ~styles=
      linkStyles
      @ Css.[
          border(`px(1), `solid, Colors'.secondary),
          Css.transition(~duration=400, "background"),
          Css.transition(~duration=400, "color"),
          selector(
            ":hover",
            [background(Colors.smokeWhite), color(Colors.darkAccentBlue)],
          ),
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
  let component = ReasonReact.statelessComponent(__MODULE__ ++ "Subheader");

  let linkItem = (~isActive=false, children: array(ReasonReact.reactElement)) => {
    let activeStyles = Css.[borderBottom(`px(2), `solid, `hex("3562AE"))];
    let activeColors = (`hex("3562AE"), `hex("3562AE"));
    let inactiveStyles =
      Css.[
        borderBottom(`px(2), `solid, `hex("3562AE00")),
        color(`hex("192D44")),
      ];
    let inactiveColors = (`hex("192D44"), `hex("192D44"));
    let allStyles =
      Css.[
        paddingBottom(`em(0.8)),
        paddingTop(`em(0.8)),
        paddingLeft(`em(0.4)),
        paddingRight(`em(0.4)),
        marginRight(`em(1.8)),
        float(`left),
      ];
    <BaseLink
      colors={isActive ? activeColors : inactiveColors}
      isDisabled=false
      styles=Css.((isActive ? activeStyles : inactiveStyles) @ allStyles)>
      ...children
    </BaseLink>;
  };

  let make = children => {
    ...component,
    render: _self =>
      <Div
        styles=[
          Css.(
            style([backgroundColor(Colors.white)] @ Base.fullWidthFloatLeft)
          ),
        ]>
        <Div
          styles=[
            Css.(
              style([
                width(`percent(100.0)),
                float(`left),
                padding2(~v=`em(0.0), ~h=`em(2.0)),
                boxShadow(
                  ~x=`px(1),
                  ~y=`px(1),
                  ~blur=`px(4),
                  `hex("00000044"),
                ),
              ])
            ),
          ]>
          ...children
        </Div>
      </Div>,
  };
};