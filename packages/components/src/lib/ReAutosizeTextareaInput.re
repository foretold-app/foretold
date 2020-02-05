[@bs.config {jsx: 3}];
let fn = (a: string) => ();

[@bs.module "./AutosizeTextareaInput.js"]
external guesstimateInput: ReasonReact.reactClass = "AutosizeTextareaInput";

[@react.component]
let make =
    (
      ~name="",
      ~placeholder="",
      ~value="",
      ~onChange=fn,
      ~inputClassName: option(string)=?,
      ~style: option(ReactDOMRe.Style.t)=?,
      ~children=ReasonReact.null,
    ) =>
  ReasonReact.wrapJsForReason(
    ~reactClass=guesstimateInput,
    ~props={
      "name": name,
      "placeholder": placeholder,
      "value": value,
      "onChange": onChange,
      "inputClassName": inputClassName,
      "style": style,
    },
    children,
  )
  |> ReasonReact.element;