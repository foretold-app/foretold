[@bs.module "./Icon.js"] external reactClass: ReasonReact.reactClass = "Icon";

// @todo: To move this component into components package.
[@react.component]
let make = (~icon=?, ~size=?, ~children=<Null />) =>
  ReasonReact.wrapJsForReason(
    ~reactClass,
    ~props={
      "iconType": icon |> E.O.default(""),
      "size": size |> E.O.default("1em"),
    },
    children,
  )
  |> ReasonReact.element;