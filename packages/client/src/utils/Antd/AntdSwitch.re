/* from: https://github.com/tiensonqin/bs-antd/blob/master/src/Antd.re */

let optBoolToOptJsBoolean =
  fun
  | None => None
  | Some(v) => Some(v);

let unwrapBool = v => Js.Undefined.fromOption @@ optBoolToOptJsBoolean(v);

[@bs.module "../../../node_modules/antd/lib/switch"]
external switch': ReasonReact.reactClass = "default";
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
    ~reactClass=switch',
    ~props=
      Js.Undefined.{
        "defaultChecked": unwrapBool(defaultChecked),
        "disabled": unwrapBool(disabled),
        "checkedChildren": fromOption(checkedChildren),
        "className": fromOption(className),
        "size": fromOption(size),
        "style": fromOption(style),
        "checked": unwrapBool(checked),
        "id": fromOption(id),
        "unCheckedChildren": fromOption(unCheckedChildren),
        "onChange": fromOption(onChange),
      },
  );