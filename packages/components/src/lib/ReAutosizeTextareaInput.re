let fn = (a: string) => ();
let fn2 = () => ();

[@bs.module "./AutosizeTextareaInput.js"]
external guesstimateInput: ReasonReact.reactClass = "AutosizeTextareaInput";

[@react.component]
let make =
    (
      ~rows: int=?,
      ~maxRows: int=?,
      ~minRows: int=?,
      ~useCacheForDOMMeasurements: bool=?,
      ~value="",
      ~onChange=fn,
      ~onHeightChange=fn2,
      ~className: option(string)=?,
      ~style: option(ReactDOMRe.Style.t)=?,
      ~children=ReasonReact.null,
    ) =>
  ReasonReact.wrapJsForReason(
    ~reactClass=guesstimateInput,
    ~props={
      "rows": rows,
      "maxRows": maxRows,
      "minRows": minRows,
      "useCacheForDOMMeasurements": useCacheForDOMMeasurements,
      "value": value,
      "onChange": onChange,
      "onHeightChange": onHeightChange,
      "className": className,
      "style": style,
    },
    children,
  )
  |> ReasonReact.element;