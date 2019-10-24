[@bs.config {jsx: 3}];

module Styles = {
  open Css;
  let link =
    style([
      color(`hex("324ea2")),
      selector(":hover", [color(`hex("000"))]),
    ]);
};

[@react.component]
let make =
    (
      ~linkType: LinkType.t,
      ~className: option(string)=?,
      ~children: array(ReasonReact.reactElement),
    ) =>
  <a
    href={LinkType.toString(linkType)}
    className={Css.merge([Styles.link, className |> E.O.default("")])}
    onClick={LinkType.onClick(linkType)}>
    {children |> ReasonReact.array}
  </a>;

module Jsx2 = {
  let make =
      (
        ~linkType: LinkType.t,
        ~className: option(string)=?,
        children: array(ReasonReact.reactElement),
      ) =>
    ReasonReactCompat.wrapReactForReasonReact(
      make,
      makeProps(~linkType, ~className?, ~children, ()),
      children,
    );
};
