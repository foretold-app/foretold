[@bs.config {jsx: 3}];

[@bs.module] external reactClass: ReasonReact.reactClass = "antd/lib/alert";

[%bs.raw {|require("antd/lib/alert/style")|}];

[@bs.obj]
external makeProps: (~message: string=?, ~_type: string=?, unit) => _ = "";

[@react.component]
let make = (~message=?, ~type_=?, ~children=ReasonReact.null) =>
  ReasonReact.wrapJsForReason(
    ~reactClass,
    ~props=makeProps(~message?, ~_type=?type_, ()),
    children,
  )
  |> ReasonReact.element;
