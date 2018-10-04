let fn = (a: (array(float), array(float))) => ();

[@bs.module "./GuesstimateInput.js"]
external guesstimateInput : ReasonReact.reactClass = "GuesstimateInput";

let make = (~sampleCount=1000, ~onUpdate=fn, children) =>
  ReasonReact.wrapJsForReason(
    ~reactClass=guesstimateInput,
    ~props={"sampleCount": sampleCount, "onUpdate": onUpdate},
    children,
  );