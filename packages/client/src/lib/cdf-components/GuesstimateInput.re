[@bs.config {jsx: 3}];

type input = {
  .
  "name": string,
  "xs": array(float),
  "ys": array(float),
};

let fn = (a: (array(float), array(float), bool)) => ();
let fn2 = (a: string) => ();

[@bs.module "./GuesstimateInput.js"]
external guesstimateInput: ReasonReact.reactClass = "GuesstimateInput";

[@react.component]
let make =
    (
      ~sampleCount=10000,
      ~min=None,
      ~max=None,
      ~inputs: array(input)=[||],
      ~onUpdate=fn,
      ~onChange=fn2,
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