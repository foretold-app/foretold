[@bs.module] external reactClass: ReasonReact.reactClass = "antd/lib/alert";

[%bs.raw {|require("antd/lib/alert/style")|}];

[@react.component]
let make = (~message=?, ~type_=?, ~children=<Null />) =>
  ReasonReact.wrapJsForReason(
    ~reactClass,
    ~props=makeProps(~message?, ~type_?, ()),
    children,
  )
  |> ReasonReact.element;