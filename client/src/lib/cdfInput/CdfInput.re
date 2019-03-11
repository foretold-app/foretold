open E;
open Utils;
open Antd;

type state = {
  floatCdf: FloatCdf.t,
  competitorType: string,
  dataType: string,
  description: string,
};

type action =
  | UpdateFloatPdf(FloatCdf.t)
  | UpdateCompetitorType(string)
  | UpdateDataType(string)
  | UpdateDescription(string);

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
  let select = style([marginBottom(px(7))]);
  let label = style([color(hex("888"))]);
};

let competitorType = (~state, ~send) =>
  <Select
    value={state.competitorType}
    onChange={e => send(UpdateCompetitorType(e))}>
    <Select.Option value="COMPETITIVE"> {"Competitive" |> ste} </Select.Option>
    <Select.Option value="OBJECTIVE"> {"Objective" |> ste} </Select.Option>
  </Select>;

let dataType = (~state, ~send) =>
  <Select
    value={state.dataType}
    onChange={e => send(UpdateDataType(e))}
    /* TODO: ADD BACK */
    /* <Select.Option value="FLOAT_CDF"> {"Distribution" |> ste} </Select.Option>
       <Select.Option value="FLOAT"> {"Point" |> ste} </Select.Option> */
  />;

let getIsValid = state =>
  switch (state.dataType) {
  | "FLOAT_CDF" => E.A.length(state.floatCdf.xs) > 1
  | _ => E.A.length(state.floatCdf.xs) == 1
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

let mainn = (~state, ~isCreator, ~send, ~onSubmit) => {
  let isValid = getIsValid(state);
  <div className=Styles.form>
    <div className=Styles.chartSection>
      {
        E.A.length(state.floatCdf.xs) > 1 ?
          <InputChart
            data={
              state.floatCdf
              |> (e => (e.xs, e.ys))
              |> Value.FloatCdf.fromArrays
              |> Value.toPdf(~bucketSize=20)
              |> Value.FloatCdf.toPoints
            }
          /> :
          <div />
      }
    </div>
    <div className=Styles.inputSection>
      {
        showIf(
          isCreator,
          <div className=Styles.select>
            {competitorType(~state, ~send)}
          </div>,
        )
      }
      {
        showIf(
          state.competitorType == "OBJECTIVE",
          <div className=Styles.select> {dataType(~state, ~send)} </div>,
        )
      }
      <div className=Styles.inputBox>
        <h4 className=Styles.label> {"Value" |> ste} </h4>
        <GuesstimateInput
          sampleCount=1000
          onUpdate={
            e =>
              {
                let (ys, xs) = e;
                let asGroup: FloatCdf.t = {xs, ys};
                send(UpdateFloatPdf(asGroup));
              }
              |> ignore
          }
        />
      </div>
      <div className=Styles.inputBox>
        <h4 className=Styles.label> {"Reasoning" |> ste} </h4>
      </div>
      <Input.TextArea
        value={state.description}
        onChange={
          event => {
            let value =
              ReactDOMRe.domElementToObj(ReactEventRe.Form.target(event))##value;
            send(UpdateDescription(value));
          }
        }
      />
      <div className=Styles.submitButton>
        <Antd.Button
          _type=`primary onClick={_ => onSubmit()} disabled={!isValid}>
          {"Submit" |> ste}
        </Antd.Button>
      </div>
    </div>
  </div>;
};

let make =
    (
      ~data: CreateMeasurementMutation.Mutation.renderPropObj,
      ~onUpdate=_ => (),
      ~isCreator=false,
      ~onSubmit=_ => (),
      _children,
    ) => {
  ...component,
  initialState: () => {
    floatCdf: FloatCdf.empty,
    competitorType: "COMPETITIVE",
    dataType: "FLOAT_CDF",
    description: "",
  },
  reducer: (action, state) =>
    switch (action) {
    | UpdateFloatPdf((e: FloatCdf.t)) =>
      onUpdate(e);
      ReasonReact.Update({...state, floatCdf: e});
    | UpdateCompetitorType(e) =>
      ReasonReact.Update({...state, competitorType: e})
    | UpdateDataType(e) => ReasonReact.Update({...state, dataType: e})
    | UpdateDescription(e) => ReasonReact.Update({...state, description: e})
    },
  render: ({state, send}) => {
    let onSubmit = () => {
      let value = getValue(state);
      onSubmit((
        value,
        getCompetitorType(state.competitorType),
        state.description,
      ));
      ();
    };
    <Style.BorderedBox>
      {
        switch (data.result) {
        | Loading => <div> {"Loading" |> ste} </div>
        | Error(e) =>
          <div>
            {"Error: " ++ e##message |> ste}
            {mainn(~state, ~isCreator, ~send, ~onSubmit)}
          </div>
        | Data(_) => <h2> {"Form submitted successfully!" |> ste} </h2>
        | NotCalled => mainn(~state, ~isCreator, ~send, ~onSubmit)
        }
      }
    </Style.BorderedBox>;
  },
};