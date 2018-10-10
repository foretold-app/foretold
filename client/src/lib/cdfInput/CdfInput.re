open Types;
open Utils;
open Antd;

type competitorType = [ | `COMPETITIVE | `OBJECTIVE];

type state = {
  floatCdf,
  competitorType: string,
  dataType: string,
};

type action =
  | UpdateFloatPdf(floatCdf)
  | UpdateCompetitorType(string)
  | UpdateDataType(string);

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
  let select = style([marginBottom(px(20))]);
};

let competitorType = (~state, ~send) =>
  <Select
    value=state.competitorType onChange=(e => send(UpdateCompetitorType(e)))>
    <Select.Option value="COMPETITIVE"> ("Competitive" |> ste) </Select.Option>
    <Select.Option value="OBJECTIVE"> ("Objective" |> ste) </Select.Option>
  </Select>;

let dataType = (~state, ~send) =>
  <Select value=state.dataType onChange=(e => send(UpdateDataType(e)))>
    <Select.Option value="FLOAT_CDF"> ("Distribution" |> ste) </Select.Option>
    <Select.Option value="FLOAT"> ("Point" |> ste) </Select.Option>
  </Select>;

let getIsValid = state =>
  switch (state.dataType) {
  | "FLOAT_CDF" => Array.length(state.floatCdf.xs) > 1
  | _ => Array.length(state.floatCdf.xs) == 1
  };

let getValue = state =>
  switch (state.dataType) {
  | "FLOAT_CDF" =>
    `FloatCdf(
      Value.FloatCdf.fromArrays(state.floatCdf |> (e => (e.ys, e.xs))),
    )
  | _ =>
    let point = Array.unsafe_get(state.floatCdf.xs, 0);
    `FloatPoint(point);
  };

let getCompetitorType =
  fun
  | "COMPETITIVE" => `COMPETITIVE
  | "OBJECTIVE" => `OBJECTIVE
  | _ => `OBJECTIVE;

let make = (~onUpdate=e => (), ~onSubmit=e => (), _children) => {
  ...component,
  initialState: () => {
    floatCdf: floatCdfEmpty,
    competitorType: "COMPETITIVE",
    dataType: "FLOAT_CDF",
  },
  reducer: (action, state) =>
    switch (action) {
    | UpdateFloatPdf((e: floatCdf)) =>
      onUpdate(e);
      ReasonReact.Update({...state, floatCdf: e});
    | UpdateCompetitorType(e) =>
      ReasonReact.Update({...state, competitorType: e})
    | UpdateDataType(e) => ReasonReact.Update({...state, dataType: e})
    },
  render: ({state, send}) => {
    let onSubmit = () => {
      let value = getValue(state);
      onSubmit((value, getCompetitorType(state.competitorType)));
      ();
    };
    let isValid = getIsValid(state);
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
          <div className=Styles.select> (competitorType(~state, ~send)) </div>
          (
            state.competitorType != "OBJECTIVE" ?
              ReasonReact.null :
              <div className=Styles.select> (dataType(~state, ~send)) </div>
          )
          <div className=Styles.inputBox>
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
          </div>
          <div className=Styles.submitButton>
            <Antd.Button
              _type=`primary onClick=(_ => onSubmit()) disabled=(! isValid)>
              ("Submit" |> ste)
            </Antd.Button>
          </div>
        </div>
      </div>
    </Style.BorderedBox>;
  },
};