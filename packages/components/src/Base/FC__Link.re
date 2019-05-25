let component = ReasonReact.statelessComponent(__MODULE__);

let defaultColors = (
  FC__Colors.Text.LightBackground.main,
  FC__Colors.Text.LightBackground.light,
);

let headerLink = (~className, ~isDisabled=false, ()) => {
  let primaryStyles =
    Css.(
      style([
        textDecoration(`none),
        userSelect(`none),
        color(FC__Colors.Text.LightBackground.main),
        hover([color(FC__Colors.Text.LightBackground.light)]),
      ])
    );

  let disabledStyles =
    isDisabled ? Css.(style([pointerEvents(`none), cursor(`default)])) : "";

  Css.(merge([primaryStyles, className, disabledStyles]));
};

let make = (~href="#", ~onClick=?, ~isDisabled=false, ~className="", children) => {
  ...component,
  render: _self =>
    <a
      disabled=isDisabled
      href
      className={headerLink(~className, ~isDisabled, ())}
      ?onClick>
      ...children
    </a>,
};