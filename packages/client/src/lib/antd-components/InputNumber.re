[@bs.module]
external reactClass: ReasonReact.reactClass = "antd/lib/input-number";

[%bs.raw {|require("antd/lib/input-number/style")|}];

// @todo: To move this component into components package.
[@react.component]
let make =
    (
      ~className=?,
      ~style=?,
      ~onChange=?,
      ~defaultValue=?,
      ~parser=?,
      ~formatter=?,
      ~value=?,
      ~min=?,
      ~max=?,
      ~step=?,
      ~disabled=false,
      ~children=<Null />,
    ) =>
  ReasonReact.wrapJsForReason(
    ~reactClass,
    ~props=
      makeProps(
        ~className?,
        ~style?,
        ~parser?,
        ~formatter?,
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
  )
  |> ReasonReact.element;