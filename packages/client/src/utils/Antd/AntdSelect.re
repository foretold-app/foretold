[@bs.config {jsx: 3}];

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

// @todo: 1

[@react.component]
let make =
    (
      ~value=?,
      ~defaultValue=?,
      ~onChange=?,
      ~className=?,
      ~children=ReasonReact.null,
    ) =>
  ReasonReact.wrapJsForReason(
    ~reactClass=select,
    ~props=makeProps(~value?, ~defaultValue?, ~onChange?, ~className?, ()),
    children,
  )
  |> ReasonReact.element;

module Option = {
  [@bs.scope "default"] [@bs.module "antd/lib/select"]
  external reactClass: ReasonReact.reactClass = "Option";

  [@bs.obj] external makeProps: (~value: string, ~title: string=?) => _ = "";

  [@react.component]
  let make = (~value, ~title=?, ~children=ReasonReact.null) =>
    ReasonReact.wrapJsForReason(
      ~reactClass,
      ~props=makeProps(~value, ~title?),
      children,
    )
    |> ReasonReact.element;
};
