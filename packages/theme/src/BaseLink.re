let component = ReasonReact.statelessComponent(__MODULE__);

let defaultColors = (Css_Colors.yellow, Css_Colors.tomato);

let headerLink = (~colors, ~styles, ~isDisabled=false, ()) => {
  let (mainColor, hoverColor) = colors;

  let disabledStyles =
    isDisabled ? Css.[pointerEvents(`none), cursor(`default)] : [];

  Css.(
    style(
      [
        color(mainColor),
        textDecoration(`none),
        userSelect(`none),
        selector(":hover", [color(hoverColor)]),
      ]
      @ styles
      @ disabledStyles,
    )
  );
};

let make =
    (
      ~href="#",
      ~onClick=?,
      ~colors=defaultColors,
      ~isDisabled=false,
      ~styles=[],
      children,
    ) => {
  ...component,
  render: _self =>
    <a
      disabled=isDisabled
      href
      className={headerLink(~colors, ~styles, ~isDisabled, ())}
      ?onClick>
      ...children
    </a>,
};