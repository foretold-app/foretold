[@bs.config {jsx: 3}];

[@bs.module "./AntdDropdown.js"]
external dropdown: ReasonReact.reactClass = "Dropdown";

[%bs.raw {|require("antd/lib/dropdown/style")|}];

[@bs.obj]
external makeProps:
  (~overlay: ReasonReact.reactElement=?, ~overlayClassName: string=?, unit) =>
  _ =
  "";

[@react.component]
let make = (~overlay=?, ~overlayClassName=?, ~children=<Null />) =>
  ReasonReact.wrapJsForReason(
    ~reactClass=dropdown,
    ~props=makeProps(~overlay?, ~overlayClassName?, ()),
    children,
  )
  |> ReasonReact.element;