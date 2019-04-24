[@bs.module "antd/lib/form"] external form: ReasonReact.reactClass = "default";

[%bs.raw {|require("antd/lib/form/style")|}];

let wrapperCol: Js.Json.t =
  {|{"xs":{"span": 24}, "sm": {"span": 12}}|} |> Json.parseOrRaise;

let labelCol: Js.Json.t =
  {|{"xs":{"span": 24}, "sm": {"span": 5}}|} |> Json.parseOrRaise;

[@bs.obj]
external makeProps:
  (
    ~onSubmit: ReactEvent.Form.t => unit=?,
    ~hideRequiredMark: bool=?,
    ~id: string=?,
    ~className: string=?,
    ~style: ReactDOMRe.Style.t=?,
    ~colon: bool=?,
    ~wrapperCol: Js.Json.t=?,
    ~labelCol: Js.Json.t=?,
    ~validateStatus: string=?,
    ~extra: string=?,
    ~required: bool=?,
    ~label: string=?,
    ~help: string=?,
    ~hasFeedback: bool=?,
    unit
  ) =>
  _ =
  "";

let make =
    (
      ~onSubmit=?,
      ~hideRequiredMark=?,
      ~id=?,
      ~className=?,
      ~style=?,
      children,
    ) =>
  ReasonReact.wrapJsForReason(
    ~reactClass=form,
    ~props=
      makeProps(
        ~onSubmit?,
        ~hideRequiredMark?,
        ~id?,
        ~wrapperCol,
        ~labelCol,
        ~className?,
        ~style?,
        (),
      ),
    children,
  );
module Item = {
  [@bs.module "antd/lib/form/FormItem"]
  external item: ReasonReact.reactClass = "default";
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
        ~help=?,
        ~hasFeedback=?,
        children,
      ) =>
    ReasonReact.wrapJsForReason(
      ~reactClass=item,
      ~props=
        makeProps(
          ~colon?,
          ~validateStatus?,
          ~extra?,
          ~className?,
          ~required?,
          ~style?,
          ~label?,
          ~id?,
          ~help?,
          ~hasFeedback?,
          (),
        ),
      children,
    );
};