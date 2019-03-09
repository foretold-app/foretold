let optBoolToOptJsBoolean =
  fun
  | None => None
  | Some(v) => Some(v);

let unwrapBool = v => Js.Undefined.from_opt @@ optBoolToOptJsBoolean(v);

[@bs.module] external form : ReasonReact.reactClass = "antd/lib/form";
let make =
    (
      ~layout=?,
      ~onSubmit=?,
      ~hideRequiredMark=?,
      ~id=?,
      ~className=?,
      ~style=?,
    ) =>
  ReasonReact.wrapJsForReason(
    ~reactClass=form,
    ~props=
      Js.Undefined.(
        {
          "layout": from_opt(layout),
          "onSubmit": from_opt(onSubmit),
          "hideRequiredMark": unwrapBool(hideRequiredMark),
          "id": from_opt(id),
          "className": from_opt(className),
          "style": from_opt(style),
        }
      ),
  );
type wrapper = (. ReasonReact.reactClass) => ReasonReact.reactClass;
[@bs.module "antd/lib/form"] external create : unit => wrapper = "create";
let wrapper = (~component, ~make', ~props, ~children) => {
  let wrapper = create();
  let reactClass' = ReasonReact.wrapReasonForJs(~component, _ => make'([||]));
  let reactClass = wrapper(. reactClass');
  ReasonReact.wrapJsForReason(~reactClass, ~props, children);
};
module Item = {
  [@bs.module "antd/lib/form"] external item : ReasonReact.reactClass = "Item";
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
      ) =>
    ReasonReact.wrapJsForReason(
      ~reactClass=item,
      ~props=
        Js.Undefined.(
          {
            "colon": unwrapBool(colon),
            "validateStatus": from_opt(validateStatus),
            "extra": from_opt(extra),
            "className": from_opt(className),
            "required": unwrapBool(required),
            "style": from_opt(style),
            "label": from_opt(label),
            "id": from_opt(id),
            "wrapperCol": from_opt(wrapperCol),
            "help": from_opt(help),
            "hasFeedback": unwrapBool(hasFeedback),
            "labelCol": from_opt(labelCol),
          }
        ),
    );
};