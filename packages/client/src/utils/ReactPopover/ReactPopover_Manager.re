[@bs.module "react-popper/lib/cjs"]
external reactClass: ReasonReact.reactClass = "Manager";

[@bs.obj] external makeProps: (~placement: string=?, unit) => _ = "";

let make = (~placement=?, children) =>
  ReasonReact.wrapJsForReason(
    ~reactClass,
    ~props=makeProps(~placement?, ()),
    children,
  );