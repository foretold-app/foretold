[@bs.module "./FC__Markdown.js"]
external markdown: ReasonReact.reactClass = "default";

module Inner = {
  [@bs.obj] external makeProps: (~source: string, unit) => _ = "";
  let make = (~source, children) =>
    ReasonReact.wrapJsForReason(
      ~reactClass=markdown,
      ~props=makeProps(~source, ()),
      children,
    );
};

let className = Css.(style([selector(".MJXc-display", [])]));

[@react.component]
let make = (~content: string, ()) =>
  <span className> <Inner source=content /> </span>;

module Jsx2 = {
  let component = ReasonReact.statelessComponent("MarkdownRenderer");

  let make = (~content: string, children) =>
    ReasonReactCompat.wrapReactForReasonReact(
      make,
      makeProps(~content, ()),
      children,
    );
};