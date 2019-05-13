let fn = (a: (array(float), array(float))) => ();

[@bs.module "./GuesstimateInput.js"]
external guesstimateInput: ReasonReact.reactClass = "GuesstimateInput";

let make = (~sampleCount=10000, ~onUpdate=fn, ~focusOnRender=true, children) =>
  ReasonReact.wrapJsForReason(
    ~reactClass=guesstimateInput,
    ~props={
      "sampleCount": sampleCount,
      "onUpdate": onUpdate,
      "focusOnRender": focusOnRender,
    },
    children,
  );