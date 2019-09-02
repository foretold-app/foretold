[@bs.module "./TimeSeriesChart.js"]
external timeSeriesChart: ReasonReact.reactClass = "TimeSeriesChart";

let make = children =>
  ReasonReact.wrapJsForReason(
    ~reactClass=timeSeriesChart,
    ~props={},
    children,
  );