[@bs.config {jsx: 3}];

[@bs.module] external reactClass: ReasonReact.reactClass = "antd/lib/input";

[%bs.raw {|require("antd/lib/input/style")|}];

[@bs.obj]
external makeProps:
  (
    ~htmlType: string=?,
    ~name: string=?,
    ~value: string=?,
    ~disabled: bool=?,
    ~defaultValue: string=?,
    ~onChange: ReactEvent.Form.t => unit=?,
    ~onPressEnter: ReactEvent.Keyboard.t => unit=?,
    ~onBlur: ReactEvent.Focus.t => unit=?,
    ~className: string=?,
    ~style: ReactDOMRe.Style.t=?,
    ~placeholder: string=?,
    unit
  ) =>
  _ =
  "";

/**
 * Antd.Input does not support some fields (disabled etc).
 */
[@react.component]
let make =
    (
      ~htmlType=?,
      ~name=?,
      ~value=?,
      ~disabled=?,
      ~defaultValue=?,
      ~onChange=?,
      ~onPressEnter=?,
      ~onBlur=?,
      ~className=?,
      ~style=?,
      ~placeholder=?,
      ~children=ReasonReact.null,
    ) =>
  ReasonReact.wrapJsForReason(
    ~reactClass,
    ~props=
      makeProps(
        ~htmlType?,
        ~name?,
        ~value?,
        ~disabled?,
        ~defaultValue?,
        ~onChange?,
        ~onPressEnter?,
        ~onBlur?,
        ~className?,
        ~style?,
        ~placeholder?,
        (),
      ),
    children,
  )
  |> ReasonReact.element;
