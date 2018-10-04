[@bs.module "./CellInputChart.js"]
external cellInputChart : ReasonReact.reactClass = "CellInputChart";

let make = (~data=[||], children) =>
  ReasonReact.wrapJsForReason(
    ~reactClass=cellInputChart,
    ~props={"data": data},
    children,
  );