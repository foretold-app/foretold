[@bs.module "react-popper/lib/cjs"]
external reactClass: ReasonReact.reactClass = "Reference";

[@bs.obj] external makeProps: (~placement: string=?, unit) => _ = "";

type referenceProps = {. "ref": Js.nullable(Dom.element) => unit};

let make = (~placement=?, children) =>
  ReasonReact.wrapJsForReason(
    ~reactClass,
    ~props=makeProps(~placement?, ()),
    children,
  );