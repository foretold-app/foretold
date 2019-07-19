let fn = (a: (array(float), array(float), bool)) => ();
let fn2 = (a: string) => ();

[@bs.module "./GuesstimateInput.js"]
external guesstimateInput: ReasonReact.reactClass = "GuesstimateInput";

let make =
    (
      ~sampleCount=10000,
      ~min=None,
      ~max=None,
      ~onUpdate=fn,
      ~onChange=fn2,
      ~focusOnRender=true,
      children,
    ) =>
  ReasonReact.wrapJsForReason(
    ~reactClass=guesstimateInput,
    ~props={
      "sampleCount": sampleCount,
      "onUpdate": onUpdate,
      "onChange": onChange,
      "min": min,
      "max": max,
      "focusOnRender": focusOnRender,
    },
    children,
  );