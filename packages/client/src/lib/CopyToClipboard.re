[@bs.module "react-copy-to-clipboard"]
external reactComponent: React.component('a) = "CopyToClipboard";

type onCopy = unit => unit;

[@bs.obj]
external makePropsText: (~text: string, ~onCopy: onCopy, ~children: 'a) => _ =
  "";

[@react.component]
let make = (~text: string, ~onCopy: onCopy, ~children) =>
  React.createElement(
    reactComponent,
    makePropsText(~text, ~onCopy, ~children),
  );