module Colors' = {
  let background = Colors.mainBlue;
  let primary = Colors.white;
  let secondary = `hex("ffffffb1");
};

module Styles = {
  open Css;
  let outer =
    style([
      width(`percent(100.0)),
      float(`left),
      backgroundColor(Colors'.background),
    ]);

  let inner =
    style([
      width(`percent(100.0)),
      float(`left),
      boxSizing(`borderBox),
      padding2(~v=`em(1.), ~h=`em(1.0)),
    ]);

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
    padding2(~v=`em(0.6), ~h=`em(3.)),
  ];

let link =
  Main.BaseLink.make(
    ~colors=(`hex("3562AE"), Colors.darkAccentBlue),
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
  Main.BaseLink.make(
    ~colors=(Colors'.secondary, Colors.darkAccentBlue),
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