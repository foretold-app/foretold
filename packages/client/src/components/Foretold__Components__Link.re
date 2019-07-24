[@bs.config {jsx: 3}];

[@react.component]
let make =
    (
      ~linkType: LinkType.t,
      ~className: option(string)=?,
      ~children: array(ReasonReact.reactElement),
    ) =>
  <a
    href={LinkType.toString(linkType)}
    ?className
    onClick={LinkType.onClick(linkType)}>
    {children |> ReasonReact.array}
  </a>;

module Jsx2 = {
  let component = ReasonReact.statelessComponent("Link");

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