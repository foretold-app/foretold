open E;
open Utils;
open Antd;
open Foretold__GraphQL;

type state = {
  // -> Measurement.value
  floatCdf: FloatCdf.t,
  dataType: string,
  // -> Measurement
  competitorType: string, // "FLOAT_CDF" | "FLOAT",
  description: string,
  valueText: string,
};

type action =
  // -> Measurement.value
  | UpdateFloatPdf(FloatCdf.t)
  | UpdateDataType(string)
  // -> Measurement
  | UpdateCompetitorType(string)
  | UpdateDescription(string)
  | UpdateValueText(string);

let component = ReasonReact.reducerComponent("CdfInput");

module Styles = {
  open Css;
  let form =
    style([display(`flex), flexDirection(`row), width(`percent(100.))]);
  let chartSection = style([flex(1)]);
  let inputSection =
    style([flex(1), marginTop(px(10)), marginRight(px(5))]);
  let inputBox = style([]);
  let submitButton = style([marginTop(px(20))]);
  let select = style([marginBottom(px(7))]);
  let label = style([color(hex("888"))]);
};

let competitorTypeSelect =
    (~state, ~send, ~measurable: Context.Primary.Measurable.t)
    : ReasonReact.reactElement => {
  let options =
    switch (measurable.state) {
    | Some(`JUDGED) => [|
        <Select.Option value="OBJECTIVE">
          {"Resolution" |> ste}
        </Select.Option>,
      |]
    | _ => [|
        <Select.Option value="COMPETITIVE">
          {"Prediction" |> ste}
        </Select.Option>,
        <Select.Option value="OBJECTIVE">
          {"Resolution" |> ste}
        </Select.Option>,
      |]
    };

  <Select
    value={state.competitorType}
    onChange={e => send(UpdateCompetitorType(e))}>
    {options |> ReasonReact.array}
  </Select>;
};

let dataTypeSelect = (~state, ~send): ReasonReact.reactElement =>
  <Select value={state.dataType} onChange={e => send(UpdateDataType(e))}>
    <Select.Option value="FLOAT_CDF"> {"Distribution" |> ste} </Select.Option>
    <Select.Option value="FLOAT_POINT"> {"Point" |> ste} </Select.Option>
  </Select>;

let getIsValid = (state: state): bool =>
  switch (state.dataType) {
  | "FLOAT_CDF" => E.A.length(state.floatCdf.xs) > 1
  | "FLOAT_POINT" => E.A.length(state.floatCdf.xs) == 1
  };

let getValue = (state: state): MeasurementValue.t =>
  switch (state.dataType) {
  | "FLOAT_CDF" =>
    `FloatCdf(
      MeasurementValue.FloatCdf.fromArrays(
        state.floatCdf |> (e => (e.ys, e.xs)),
      ),
    )
  | "FLOAT_POINT" =>
    let point = Array.unsafe_get(state.floatCdf.xs, 0);
    `FloatPoint(point);
  };

let getCompetitorType = (str: string) =>
  switch (str) {
  | "COMPETITIVE" => `COMPETITIVE
  | "OBJECTIVE" => `OBJECTIVE
  | _ => `OBJECTIVE
  };

let mainBlock =
    (
      ~state: state,
      ~isCreator: bool,
      ~send,
      ~onSubmit,
      ~measurable: Context.Primary.Measurable.t,
    )
    : ReasonReact.reactElement => {
  let isValid = getIsValid(state);

  let getDataTypeSelect = (): ReasonReact.reactElement =>
    switch (state.competitorType, measurable.valueType) {
    | ("OBJECTIVE", `FLOAT | `DATE) =>
      <div className=Styles.select> {dataTypeSelect(~state, ~send)} </div>
    | _ => ReasonReact.null
    };

  let getValueInput = (): ReasonReact.reactElement =>
    switch (state.competitorType, measurable.valueType) {
    | ("OBJECTIVE" | "COMPETITIVE", `FLOAT | `DATE) =>
      <GuesstimateInput
        focusOnRender=true
        sampleCount=30000
        onUpdate={event =>
          {
            let (ys, xs) = event;
            let asGroup: FloatCdf.t = {xs, ys};
            send(UpdateFloatPdf(asGroup));
          }
          |> ignore
        }
        onChange={text => send(UpdateValueText(text))}
      />
    | ("OBJECTIVE", `PERCENTAGE) =>
      <Select value={state.dataType} onChange={e => send(UpdateDataType(e))}>
        <Select.Option value="TRUE"> {"True" |> ste} </Select.Option>
        <Select.Option value="FALSE"> {"False" |> ste} </Select.Option>
      </Select>
    | ("COMPETITIVE", `PERCENTAGE) => "Value 0. .. 100." |> Utils.ste
    | _ => ReasonReact.null
    };

  <div className=Styles.form>
    <div className=Styles.chartSection>
      {E.A.length(state.floatCdf.xs) > 1
         ? <LargeCdfChart
             data={
               state.floatCdf
               |> (e => (e.xs, e.ys))
               |> MeasurementValue.FloatCdf.fromArrays
               |> MeasurementValue.toPdf
               |> MeasurementValue.FloatCdf.toJs
             }
           />
         : <div />}
    </div>
    <div className=Styles.inputSection>
      {E.React.showIf(
         isCreator,
         <div className=Styles.select>
           {competitorTypeSelect(~state, ~send, ~measurable)}
         </div>,
       )}
      {getDataTypeSelect()}
      <div className=Styles.inputBox>
        <h4 className=Styles.label> {"Value" |> ste} </h4>
        {getValueInput()}
      </div>
      <div className=Styles.inputBox>
        <h4 className=Styles.label> {"Reasoning" |> ste} </h4>
      </div>
      <Input.TextArea
        value={state.description}
        onChange={event => {
          let value =
            ReactDOMRe.domElementToObj(ReactEventRe.Form.target(event))##value;
          send(UpdateDescription(value));
        }}
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
      ~data: Mutations.MeasurementCreate.Mutation.renderPropObj,
      ~onUpdate=_ => (),
      ~isCreator=false,
      ~onSubmit=_ => (),
      ~measurable: Context.Primary.Measurable.t,
      _children,
    ) => {
  ...component,

  initialState: () => {
    let competitorTypeInitValue =
      switch (measurable.state) {
      | Some(`JUDGED) => "OBJECTIVE"
      | _ => "COMPETITIVE"
      };

    {
      floatCdf: FloatCdf.empty,
      competitorType: competitorTypeInitValue,
      dataType: "FLOAT_CDF",
      description: "",
      valueText: "",
    };
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

    | UpdateValueText(e) => ReasonReact.Update({...state, valueText: e})
    },

  render: ({state, send}) => {
    let onSubmit = () => {
      let value = getValue(state);
      onSubmit((
        value,
        getCompetitorType(state.competitorType),
        state.description,
        state.valueText,
      ));
      ();
    };

    <Style.BorderedBox>
      {switch (data.result) {
       | Loading => "Loading" |> ste
       | Error(e) =>
         <>
           {"Error: " ++ e##message |> ste}
           {mainBlock(~state, ~isCreator, ~send, ~onSubmit, ~measurable)}
         </>
       | Data(_) => "Form submitted successfully!" |> ste |> E.React.inH2
       | NotCalled =>
         mainBlock(~state, ~isCreator, ~send, ~onSubmit, ~measurable)
       }}
    </Style.BorderedBox>;
  },
};