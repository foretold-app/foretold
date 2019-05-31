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

let varantColors = (variant: variant) =>
  FC__Colors.(
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

let styles =
    (
      ~isDisabled=false,
      ~variant: variant,
      ~heightPadding=4,
      ~fullWidth=false,
      (),
    ) => {
  let colors = varantColors(variant);
  let main =
    Css.(
      style([
        padding2(~v=`px(heightPadding), ~h=`px(14)),
        FC__BaseStyles.floatLeft,
        borderRadius(FC__Colors.BorderRadius.medium),
        border(`px(1), `solid, `hex(colors.border)),
        color(`hex(colors.text)),
        background(`hex(colors.background)),
        hover([
          background(`hex(colors.backgroundHover)),
          color(`hex(colors.textHover)),
        ]),
        transition(
          ~duration=FC__Colors.Transitions.standardLength,
          "background",
        ),
      ])
    );
  let disabledStyles =
    Css.(style([background(FC__Colors.greydisabled), opacity(0.5)]));
  let fullWidthStyle =
    Css.(
      style([
        width(`percent(100.)),
        boxSizing(`borderBox),
        textAlign(`center),
        Css.float(`none),
        display(`block)
      ])
    );

  switch (isDisabled, fullWidth) {
  | (false, false) => main
  | (true, false) => Css.merge([main, disabledStyles])
  | (false, true) => Css.merge([main, fullWidthStyle])
  | (true, true) => Css.merge([main, disabledStyles, fullWidthStyle])
  };
};

let component = ReasonReact.statelessComponent(__MODULE__);

let make =
    (
      ~href=?,
      ~onClick=?,
      ~variant=Secondary,
      ~isDisabled=false,
      ~fullWidth=false,
      ~className="",
      children,
    ) => {
  ...component,
  render: _self =>
    <FC__Link
      ?href
      ?onClick
      isDisabled
      className={Css.merge([
        styles(~isDisabled, ~fullWidth, ~variant, ()),
        className,
      ])}>
      ...children
    </FC__Link>,
};
