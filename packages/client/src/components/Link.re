module Styles = {
  open Css;
  let link =
    style([
      color(`hex("324ea2")),
      selector(":hover", [color(`hex("000"))]),
    ]);
};

// @todo: To use link from components folder.
[@react.component]
let make =
    (~linkType: LinkType.t, ~className: option(string)=?, ~children=<Null />) =>
  <a
    href={LinkType.toString(linkType)}
    className={Css.merge([Styles.link, className |> E.O.default("")])}
    onClick={LinkType.onClick(linkType)}>
    children
  </a>;