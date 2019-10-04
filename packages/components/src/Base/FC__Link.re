[@bs.config {jsx: 3}];

let headerLink = (~className, ~isDisabled=false, ()) => {
  let primaryStyles =
    Css.(
      style([
        textDecoration(`none),
        userSelect(`none),
        color(FC__Settings.Text.LightBackground.main),
        hover([color(FC__Settings.Text.LightBackground.light)]),
        focus([textDecoration(`none)]),
      ])
    );

  let disabledStyles =
    isDisabled ? Css.(style([pointerEvents(`none), cursor(`default)])) : "";

  Css.(merge([primaryStyles, className, disabledStyles]));
};

[@react.component]
let make =
    (~href="#", ~onClick=?, ~isDisabled=false, ~className="", ~children) => {
  <a
    disabled=isDisabled
    href
    className={headerLink(~className, ~isDisabled, ())}
    ?onClick>
    children
  </a>;
};

module Jsx2 = {
  let component = ReasonReact.statelessComponent("Link");

  let make = (~href=?, ~onClick=?, ~isDisabled=?, ~className=?, children) =>
    ReasonReactCompat.wrapReactForReasonReact(
      make,
      makeProps(
        ~href?,
        ~onClick?,
        ~isDisabled?,
        ~className?,
        ~children=children |> ReasonReact.array,
        (),
      ),
      children,
    );
};