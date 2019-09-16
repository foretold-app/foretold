type color = [ | `hex(Js.String.t)];

/* I made this contain strings instead of colors,
   because the type for background is different than
   that for the others, which made things pretty messy. */

type variantColors = {
  text: string,
  textHover: string,
  border: string,
  background: string,
  backgroundHover: string,
};

type variant =
  | Primary
  | Secondary;

type size =
  | Small
  | Median
  | Large;

let varantColors = (variant: variant) =>
  FC__Settings.(
    switch (variant) {
    | Primary => {
        text: white |> toS,
        textHover: white |> toS,
        border: link |> toS,
        background: link |> toS,
        backgroundHover: linkHover |> toS,
      }
    | Secondary => {
        text: textDark |> toS,
        textHover: textDark |> toS,
        border: accentBlueO8 |> toS,
        background: white |> toS,
        backgroundHover: buttonHover |> toS,
      }
    }
  );

let sizeStyles = size => {
  switch (size) {
  | Small =>
    Css.(style([padding2(~v=`em(0.3), ~h=`em(1.2)), fontSize(`px(14))]))
  | Median =>
    Css.(style([padding2(~v=`em(0.4), ~h=`em(1.8)), fontSize(`px(16))]))
  | Large =>
    Css.(style([padding2(~v=`em(0.5), ~h=`em(2.4)), fontSize(`px(16))]))
  };
};

let styles = (~isDisabled=false, ~variant, ~size, ~fullWidth=false, ()) => {
  let colors = varantColors(variant);

  let main =
    Css.(
      style([
        fontFamily(FC__Settings.Text.standardFont),
        fontSize(`px(16)),
        lineHeight(`em(1.5)),
        cursor(`pointer),
        FC__BaseStyles.floatLeft,
        borderRadius(FC__Settings.BorderRadius.medium),
        border(`px(1), `solid, `hex(colors.border)),
        color(`hex(colors.text)),
        background(`hex(colors.background)),
        hover([
          background(`hex(colors.backgroundHover)),
          color(`hex(colors.textHover)),
        ]),
        transition(
          ~duration=FC__Settings.Transitions.standardLength,
          "background",
        ),
      ])
    );

  let disabledStyles =
    Css.(style([background(FC__Settings.greydisabled), opacity(0.5)]));

  let fullWidthStyle =
    Css.(
      style([
        width(`percent(100.)),
        boxSizing(`borderBox),
        textAlign(`center),
        Css.float(`none),
        display(`block),
      ])
    );

  let total =
    switch (isDisabled, fullWidth) {
    | (false, false) => main
    | (true, false) => Css.merge([main, disabledStyles])
    | (false, true) => Css.merge([main, fullWidthStyle])
    | (true, true) => Css.merge([main, disabledStyles, fullWidthStyle])
    };

  Css.merge([sizeStyles(size), total]);
};

// Button must be button
let component = ReasonReact.statelessComponent(__MODULE__);
let make =
    (
      ~onClick=?,
      ~variant=Secondary,
      ~size=Median,
      ~isDisabled=false,
      ~fullWidth=false,
      ~className="",
      children,
    ) => {
  ...component,
  render: _self =>
    <button
      ?onClick
      disabled=isDisabled
      className={Css.merge([
        styles(~isDisabled, ~fullWidth, ~variant, ~size, ()),
        className,
      ])}>
      ...children
    </button>,
};