let fn = (a: (array(float), array(float))) => ();

[@bs.module "./Cell.js"]
external victoryMeasurement : ReasonReact.reactClass = "Cell";

let make = (~sampleCount=1000, ~cdfCount=100, ~onUpdate=fn, children) =>
  ReasonReact.wrapJsForReason(
    ~reactClass=victoryMeasurement,
    ~props={
      "sampleCount": sampleCount,
      "cdfCount": cdfCount,
      "onUpdate": onUpdate,
    },
    children,
  );