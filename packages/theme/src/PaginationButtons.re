type directionButton = {
  isDisabled: bool,
  onClick: ReactEvent.Mouse.t => unit,
};

type currentValue =
  | Item(int)
  | Range(int, int);

type t = {
  currentValue,
  max: int,
  pageLeft: directionButton,
  pageRight: directionButton,
};

let _text = (t: t) => {
  let currentValue =
    switch (t.currentValue) {
    | Item(i) => i |> string_of_int
    | Range(a, b) => (a |> string_of_int) ++ " - " ++ (b |> string_of_int)
    };
  currentValue ++ " of " ++ (t.max |> string_of_int);
};

let _directionLink = (t: directionButton, icon: string, positionStyles) =>
  <BaseLink
    colors=(Colors.darkAccentBlue, Colors.black)
    isDisabled={t.isDisabled}
    onClick={t.onClick}
    styles={
      Simple.tagStyles(~heightPadding=5, ~isDisabled=t.isDisabled, ())
      @ positionStyles
    }>
    {icon |> ReasonReact.string}
  </BaseLink>;

let make = (t: t) => {
  let textStyle =
    Css.[
      Base.floatLeft,
      marginRight(`em(0.5)),
      marginTop(`em(0.4)),
      color(Colors.accentBlue),
    ];
  <Div
    styles=[
      Css.(style([Base.floatLeft, padding2(~v=`em(0.7), ~h=`em(1.5))])),
    ]>
    <span className={Css.style(textStyle)}>
      {_text(t) |> ReasonReact.string}
    </span>
    {_directionLink(t.pageLeft, "<", [])}
    {_directionLink(t.pageRight, ">", [Css.marginLeft(`em(0.3))])}
  </Div>;
};