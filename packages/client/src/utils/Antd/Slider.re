[@bs.module] external reactClass: ReasonReact.reactClass = "antd/lib/slider";

[%bs.raw {|require("antd/lib/slider/style")|}];

[@bs.obj]
external makeProps:
  (
    ~className: string=?,
    ~style: ReactDOMRe.Style.t=?,
    ~disabled: bool,
    ~defaultValue: float=?,
    ~value: float=?,
    ~min: float=?,
    ~max: float=?,
    ~step: float=?,
    ~onChange: float => unit=?,
    unit
  ) =>
  _ =
  "";

let make =
    (
      ~className=?,
      ~style=?,
      ~onChange=?,
      ~defaultValue=?,
      ~value=?,
      ~min=?,
      ~max=?,
      ~step=?,
      ~disabled=false,
      children,
    ) =>
  ReasonReact.wrapJsForReason(
    ~reactClass,
    ~props=
      makeProps(
        ~className?,
        ~style?,
        ~onChange?,
        ~defaultValue?,
        ~value?,
        ~min?,
        ~max?,
        ~step?,
        ~disabled,
        (),
      ),
    children,
  );