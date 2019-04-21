[@bs.module "./AntdDropdown2.js"]
external dropdown: ReasonReact.reactClass = "Dropdown";

[@bs.obj]
external makeProps: (~overlay: ReasonReact.reactElement=?, unit) => _ = "";

let make = (~overlay=?, children) =>
  ReasonReact.wrapJsForReason(
    ~reactClass=dropdown,
    ~props=makeProps(~overlay?, ()),
    children,
  );