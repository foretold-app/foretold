// from https://github.com/JakubMarkiewicz/bs-ant-design/blob/master/src/Antd_DatePicker.re

[@bs.module]
external reactClass: ReasonReact.reactClass = "antd/lib/date-picker";

[%bs.raw {|require("antd/lib/date-picker/style")|}];

[@bs.obj]
external makeProps:
  (
    ~className: string=?,
    ~style: ReactDOMRe.Style.t=?,
    ~disabled: bool,
    ~value: MomentRe.Moment.t=?,
    ~onChange: MomentRe.Moment.t => unit=?,
    unit
  ) =>
  _ =
  "";

[@react.component]
let make =
    (
      ~className=?,
      ~style=?,
      ~onChange=?,
      ~value=?,
      ~disabled=false,
      ~children=<Null />,
    ) =>
  ReasonReact.wrapJsForReason(
    ~reactClass,
    ~props=
      makeProps(~className?, ~style?, ~onChange?, ~value?, ~disabled, ()),
    children,
  )
  |> ReasonReact.element;