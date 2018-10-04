open Types;
open Utils;

type action =
  | UpdateFloatPdf(floatCdf);

type state = {floatCdf};

let component = ReasonReact.reducerComponent("CdfInput");

module Styles = {
  open Css;
  let form =
    style([display(`flex), flexDirection(`row), width(`percent(100.))]);
  let chartSection = style([flex(2)]);
  let inputSection =
    style([flex(1), marginTop(px(10)), marginRight(px(5))]);
  let inputBox = style([]);
  let submitButton = style([marginTop(px(20))]);
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
    <Style.BorderedBox>
      <div className=Styles.form>
        <div className=Styles.chartSection>
          (
            Array.length(state.floatCdf.xs) > 1 ?
              <InputChart
                data=(
                  state.floatCdf
                  |> (e => (e.xs, e.ys))
                  |> Value.FloatCdf.fromArrays
                  |> Value.toPdf(~bucketSize=20)
                  |> Value.FloatCdf.toPoints
                )
              /> :
              <div />
          )
        </div>
        <div className=Styles.inputSection>
          <div className=Styles.inputBox>
            <GuesstimateInput
              sampleCount=5000
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
          </div>
          <div className=Styles.submitButton>
            <Antd.Button
              _type=`primary onClick=(_ => onSubmit(state.floatCdf))>
              ("Submit" |> ste)
            </Antd.Button>
          </div>
        </div>
      </div>
    </Style.BorderedBox>,
};