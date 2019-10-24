[@bs.config {jsx: 3}];

[@bs.module] external reactClass: ReasonReact.reactClass = "antd/lib/alert";

[%bs.raw {|require("antd/lib/alert/style")|}];

// @todo: 1

[@react.component]
let make = (~message=?, ~type_=?, ~children=ReasonReact.null) =>
  ReasonReact.wrapJsForReason(
    ~reactClass,
    ~props=makeProps(~message?, ~type_?, ()),
    children,
  )
  |> ReasonReact.element;
