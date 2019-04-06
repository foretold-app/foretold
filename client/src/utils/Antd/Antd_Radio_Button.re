[@bs.module "antd/lib/radio/radioButton"]
external reactClass: ReasonReact.reactClass = "default";

[@bs.obj] external makeProps: (~value: string, unit) => _ = "";

let make = (~value, children) =>
  ReasonReact.wrapJsForReason(
    ~reactClass,
    ~props=makeProps(~value, ()),
    children,
  );