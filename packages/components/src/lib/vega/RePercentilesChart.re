[@bs.module "./PercentilesChart.js"]
external percentilesChart: ReasonReact.reactClass = "PercentilesChart";

[@react.component]
let make = (~children=ReasonReact.null) =>
  ReasonReact.wrapJsForReason(
    ~reactClass=percentilesChart,
    ~props={},
    children,
  )
  |> ReasonReact.element;