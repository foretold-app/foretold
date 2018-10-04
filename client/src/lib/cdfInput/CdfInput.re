open Types;
open Utils;

type action =
  | UpdateFloatPdf(floatCdf);

type state = {floatCdf};

let component = ReasonReact.reducerComponent("CdfInput");

module Styles = {
  open Css;
  let form =
    style([
      maxWidth(px(1500)),
      display(`flex),
      flexDirection(`row),
      padding(px(3)),
      backgroundColor(hex("f6f6f6")),
      borderRadius(px(2)),
    ]);
  let chart = style([flex(2)]);
  let input = style([flex(1), marginTop(px(10)), marginRight(px(5))]);
};

let make = (~onUpdate=e => (), ~onSubmit=e => (), _children) => {
  ...component,
  initialState: () => {floatCdf: floatCdfEmpty},
  reducer: (action, state) =>
    switch (action) {
    | UpdateFloatPdf((e: floatCdf)) =>
      onUpdate(e);
      ReasonReact.Update({floatCdf: e});
    },
  render: ({state, send}) =>
    <div className=Styles.form>
      <div className=Styles.chart>
        <InputChart
          data=(
            state.floatCdf
            |> (e => (e.xs, e.ys))
            |> Value.FloatCdf.fromArrays
            |> Value.FloatCdf.toPoints
          )
        />
      </div>
      <div className=Styles.input>
        <GuesstimateInput
          sampleCount=1000
          onUpdate=(
            e =>
              {
                let (ys, xs) = e;
                let asGroup: floatCdf = {xs, ys};
                send(UpdateFloatPdf(asGroup));
              }
              |> ignore
          )
        />
        <Antd.Button _type=`primary onClick=(_ => onSubmit(state.floatCdf))>
          ("Submit" |> ste)
        </Antd.Button>
      </div>
    </div>,
};