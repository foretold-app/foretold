[@bs.module]
external reactClass : ReasonReact.reactClass = "antd/lib/date-picker/index";

[%bs.raw {|require("antd/lib/date-picker/style")|}];

[@bs.obj]
external makeProps :
  (~className: string=?, ~style: ReactDOMRe.Style.t=?, unit) => _ =
  "";

let make = (~className=?, ~style=?, children) =>
  ReasonReact.wrapJsForReason(
    ~reactClass,
    ~props=makeProps(~className?, ~style?, ()),
    children,
  );