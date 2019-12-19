let optBoolToOptJsBoolean =
  fun
  | None => None
  | Some(v) => Some(v);

let unwrapBool = v => Js.Undefined.fromOption @@ optBoolToOptJsBoolean(v);

[@bs.module] external form: ReasonReact.reactClass = "antd/lib/form";

[@react.component]
let make =
    (
      ~layout=?,
      ~onSubmit=?,
      ~hideRequiredMark=?,
      ~id=?,
      ~className=?,
      ~style=?,
      ~children=<Null />,
    ) =>
  ReasonReact.wrapJsForReason(
    ~reactClass=form,
    ~props=
      Js.Undefined.{
        "layout": fromOption(layout),
        "onSubmit": fromOption(onSubmit),
        "hideRequiredMark": unwrapBool(hideRequiredMark),
        "id": fromOption(id),
        "className": fromOption(className),
        "style": fromOption(style),
      },
    children,
  )
  |> ReasonReact.element;

type wrapper = (. ReasonReact.reactClass) => ReasonReact.reactClass;

[@bs.module "antd/lib/form"] external create: unit => wrapper = "create";
let wrapper = (~component, ~make', ~props, ~children=<Null />) => {
  let wrapper = create();
  let reactClass' = ReasonReact.wrapReasonForJs(~component, _ => make'([||]));
  let reactClass = wrapper(. reactClass');
  ReasonReact.wrapJsForReason(~reactClass, ~props, children)
  |> ReasonReact.element;
};

module Item = {
  [@bs.module "antd/lib/form"] external item: ReasonReact.reactClass = "Item";

  [@react.component]
  let make =
      (
        ~colon=?,
        ~validateStatus=?,
        ~extra=?,
        ~className=?,
        ~required=?,
        ~style=?,
        ~label=?,
        ~id=?,
        ~wrapperCol=?,
        ~help=?,
        ~hasFeedback=?,
        ~labelCol=?,
        ~children=<Null />,
      ) =>
    ReasonReact.wrapJsForReason(
      ~reactClass=item,
      ~props=
        Js.Undefined.{
          "colon": unwrapBool(colon),
          "validateStatus": fromOption(validateStatus),
          "extra": fromOption(extra),
          "className": fromOption(className),
          "required": unwrapBool(required),
          "style": fromOption(style),
          "label": fromOption(label),
          "id": fromOption(id),
          "wrapperCol": fromOption(wrapperCol),
          "help": fromOption(help),
          "hasFeedback": unwrapBool(hasFeedback),
          "labelCol": fromOption(labelCol),
        },
      children,
    )
    |> ReasonReact.element;
};