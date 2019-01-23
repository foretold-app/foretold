/* from: https://github.com/tiensonqin/bs-antd/blob/master/src/Antd.re */

let optBoolToOptJsBoolean =
  fun
  | None => None
  | Some(v) => Some(v);

let unwrapBool = v => Js.Undefined.from_opt @@ optBoolToOptJsBoolean(v);

[@bs.module]
external switch_: ReasonReact.reactClass =
  "../../../node_modules/antd/lib/switch";
%bs.raw
"require('../../../node_modules/antd/lib/switch/style/index.css')";

let make =
    (
      ~defaultChecked=?,
      ~disabled=?,
      ~checkedChildren=?,
      ~className=?,
      ~size=?,
      ~style=?,
      ~checked=?,
      ~id=?,
      ~unCheckedChildren=?,
      ~onChange=?,
    ) =>
  ReasonReact.wrapJsForReason(
    ~reactClass=switch__,
    ~props=
      Js.Undefined.{
        "defaultChecked": unwrapBool(defaultChecked),
        "disabled": unwrapBool(disabled),
        "checkedChildren": from_opt(checkedChildren),
        "className": from_opt(className),
        "size": from_opt(size),
        "style": from_opt(style),
        "checked": unwrapBool(checked),
        "id": from_opt(id),
        "unCheckedChildren": from_opt(unCheckedChildren),
        "onChange": from_opt(onChange),
      },
  );