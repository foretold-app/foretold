[@bs.module] external reactClass: ReasonReact.reactClass = "antd/lib/alert";

[%bs.raw {|require("antd/lib/alert/style")|}];

[@bs.obj]
external makeProps: (~message: string=?, ~_type: string=?, unit) => _ = "";

let make = (~message=?, ~type_=?, children) =>
  ReasonReact.wrapJsForReason(
    ~reactClass,
    ~props=makeProps(~message?, ~_type=?type_, ()),
    children,
  );