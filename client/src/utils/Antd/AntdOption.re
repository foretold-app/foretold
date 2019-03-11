/* [@bs.module "antd/lib/select"]
   external reactClass: ReasonReact.reactClass = "default.Option"; */

/* TODO: I couldn't get this to work yet :( */
[@bs.module "../../../node_modules/rc-select/lib/Select"]
external reactClass: ReasonReact.reactClass = "default";

[@bs.obj] external makeProps: (~value: string, ~title: string=?) => _ = "";

let make = (~value, ~title=?) => {
  Js.log(reactClass);
  ReasonReact.wrapJsForReason(
    ~reactClass,
    ~props=makeProps(~value, ~title?),
  );
};