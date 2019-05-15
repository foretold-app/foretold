open FC__Base;
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
  <Link
    colors=(Colors.darkAccentBlue, Colors.black)
    isDisabled={t.isDisabled}
    onClick={t.onClick}
    styles={
      FC__Simple.tagStyles(~heightPadding=5, ~isDisabled=t.isDisabled, ())
      @ positionStyles
    }>
    {icon |> ReasonReact.string}
  </Link>;

let make = (t: t) => {
  let textStyle =
    Css.[
      BaseStyles.floatLeft,
      marginRight(`em(0.5)),
      marginTop(`em(0.45)),
      color(Colors.accentBlue),
    ];
  <Div
    styles=[
      Css.(
        style([BaseStyles.floatLeft, padding2(~v=`em(0.5), ~h=`em(1.5))])
      ),
    ]>
    <span className={Css.style(textStyle)}>
      {_text(t) |> ReasonReact.string}
    </span>
    {_directionLink(t.pageLeft, "<", [])}
    {_directionLink(t.pageRight, ">", [Css.marginLeft(`em(0.3))])}
  </Div>;
};