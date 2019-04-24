[@bs.module "antd/lib/select"]
external select: ReasonReact.reactClass = "default";

[%bs.raw {|require("antd/lib/select/style")|}];

[@bs.obj]
external makeProps:
  (
    ~value: string=?,
    ~defaultValue: string=?,
    ~showSearch: bool=?,
    ~onChange: string => unit=?,
    ~className: string=?,
    unit
  ) =>
  _ =
  "";
let make = (~value=?, ~defaultValue=?, ~onChange=?, ~className=?, children) =>
  ReasonReact.wrapJsForReason(
    ~reactClass=select,
    ~props=
      makeProps(
        ~value?,
        ~defaultValue?,
        ~onChange?,
        ~className?,
        ~showSearch=true,
        (),
      ),
    children,
  );

module Option = {
  [@bs.scope "default"] [@bs.module "antd/lib/select"]
  external reactClass: ReasonReact.reactClass = "Option";

  [@bs.obj] external makeProps: (~value: string, ~title: string=?) => _ = "";

  let make = (~value, ~title=?) =>
    ReasonReact.wrapJsForReason(
      ~reactClass,
      ~props=makeProps(~value, ~title?),
    );
};