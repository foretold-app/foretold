let fn = (a: (array(float), array(float))) => ();

[@bs.module "./DynamicForm.js"]
external dynamicForm : ReasonReact.reactClass = "DynamicForm";

let make = (~sampleCount=1000, ~onUpdate=fn, children) =>
  ReasonReact.wrapJsForReason(
    ~reactClass=dynamicForm,
    ~props={"sampleCount": sampleCount, "onUpdate": onUpdate},
    children,
  );