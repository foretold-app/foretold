[@bs.module "antd/lib/dropdown"]
external reactClass: ReasonReact.reactClass = "default";

/* TODO: Fix this file! */
[%bs.raw {|require("antd/lib/dropdown/style")|}];

[@bs.obj]
external makeProps: (~overlay: ReasonReact.reactElement=?, unit) => _ = "";

let make = (~overlay=?, children) =>
  ReasonReact.wrapJsForReason(
    ~reactClass,
    ~props=makeProps(~overlay?, ()),
    children,
  );