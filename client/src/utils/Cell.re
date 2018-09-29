[@bs.module "./Cell.js"]
external victoryMeasurement : ReasonReact.reactClass = "Cell";
let make = children =>
  ReasonReact.wrapJsForReason(
    ~reactClass=victoryMeasurement,
    ~props={},
    children,
  );