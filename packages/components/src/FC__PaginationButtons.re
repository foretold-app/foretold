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

module Styles = {
  let link = isDisabled => FC__Tag.styles(~heightPadding=5, ~isDisabled, ());

  let linkColors = (Colors.darkAccentBlue, Colors.black);

  let buttonLabel =
    Css.(
      style([
        BaseStyles.floatLeft,
        marginRight(`em(0.5)),
        marginTop(`em(0.45)),
        color(Colors.accentBlue),
      ])
    );

  let position =
    Css.(
      style([BaseStyles.floatLeft, padding2(~v=`em(0.5), ~h=`em(1.5))])
    );

  let rightButton = Css.(style([marginLeft(`em(0.3))]));
};

let _directionLink = (t: directionButton, icon: string, positionStyles) =>
  <FC__Button
    isDisabled={t.isDisabled} onClick={t.onClick} className=positionStyles>
    {icon |> ReasonReact.string}
  </FC__Button>;

let make = (t: t) =>
  <Div styles=[Styles.position]>
    <span className=Styles.buttonLabel>
      {_text(t) |> ReasonReact.string}
    </span>
    {_directionLink(t.pageLeft, "<", "")}
    {_directionLink(t.pageRight, ">", Styles.rightButton)}
  </Div>;