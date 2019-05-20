[@bs.module "react-popper/lib/cjs"]
external reactClass: ReasonReact.reactClass = "Popper";

[@bs.obj] external makeProps: (~placement: string=?, unit) => _ = "";

type arrowPropsType = {
  .
  "ref": Js.nullable(Dom.element) => unit,
  "style": ReactDOMRe.style,
};

type popperProps = {
  .
  "ref": Js.nullable(Dom.element) => unit,
  "style": ReactDOMRe.style,
  "placement": string,
  "arrowProps": arrowPropsType,
};

let make = (~placement=?, children) =>
  ReasonReact.wrapJsForReason(
    ~reactClass,
    ~props=makeProps(~placement?, ()),
    children,
  );