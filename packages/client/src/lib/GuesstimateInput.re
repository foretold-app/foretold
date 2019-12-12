type input = {
  .
  "name": string,
  "xs": array(float),
  "ys": array(float),
};

type sampler = {
  .
  "isRangeDistribution": bool,
  "referenceName": string,
  "displayName": string,
  "types": array(string),
};

let onUpdate = (_a: (array(float), array(float), bool), _b: sampler) => ();
let onChange = (_a: string) => ();

[@bs.module "./GuesstimateInput.js"]
external guesstimateInput: ReasonReact.reactClass = "GuesstimateInput";

[@react.component]
let make =
    (
      ~sampleCount=10000,
      ~min=None,
      ~max=None,
      ~inputs: array(input)=[||],
      ~onUpdate=onUpdate,
      ~onChange=onChange,
      ~focusOnRender=true,
      ~children=<Null />,
    ) =>
  ReasonReact.wrapJsForReason(
    ~reactClass=guesstimateInput,
    ~props={
      "sampleCount": sampleCount,
      "onUpdate": onUpdate,
      "onChange": onChange,
      "min": min,
      "max": max,
      "inputs": inputs,
      "focusOnRender": focusOnRender,
    },
    children,
  )
  |> ReasonReact.element;