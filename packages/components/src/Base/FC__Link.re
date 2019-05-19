let component = ReasonReact.statelessComponent(__MODULE__);

let defaultColors = (
  FC__Colors.Text.LightBackground.main,
  FC__Colors.Text.LightBackground.light,
);

let headerLink = (~colors, ~styles, ~isDisabled=false, ()) => {
  let (mainColor, hoverColor) = colors;

  let primaryStyles =
    Css.(
      style([
        color(mainColor),
        textDecoration(`none),
        userSelect(`none),
        selector(":hover", [color(hoverColor)]),
      ])
    );

  let disabledStyles =
    isDisabled ? Css.(style([pointerEvents(`none), cursor(`default)])) : "";

  Css.(merge([primaryStyles, styles, disabledStyles]));
};

let make =
    (
      ~href="#",
      ~onClick=?,
      ~colors=defaultColors,
      ~isDisabled=false,
      ~styles="",
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