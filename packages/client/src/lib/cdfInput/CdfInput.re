open E;
open Utils;
open Antd;
open Foretold__GraphQL;

type state = {
  // -> Measurement.value
  floatCdf: FloatCdf.t,
  percentage: float,
  binary: bool,
  dataType: string,
  unresolvableResolution: string,
  comment: string,
  // -> Measurement
  competitorType: string,
  description: string,
  valueText: string,
  hasLimitError: bool,
};

type action =
  // -> Measurement.value
  | UpdateFloatPdf(FloatCdf.t)
  | UpdateHasLimitError(bool)
  | UpdatePercentage(float)
  | UpdateBinary(bool)
  | UpdateDataType(string)
  | UpdateUnresolvableResolution(string)
  | UpdateComment(string)
  // -> Measurement
  | UpdateCompetitorType(string)
  | UpdateDescription(string)
  | UpdateValueText(string);

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
    (~state, ~send, ~measurable: Primary.Measurable.t)
    : ReasonReact.reactElement => {
  let options: array(ReasonReact.reactElement) =
    switch (measurable.state) {
    | Some(`JUDGED) => [|
        <Select.Option value="OBJECTIVE"> {"Resolve" |> ste} </Select.Option>,
      |]
    | _ => [|
        <Select.Option value="COMPETITIVE">
          {"Predict" |> ste}
        </Select.Option>,
      |]
    };

  let options': array(ReasonReact.reactElement) =
    Array.append(
      options,
      [|
        <Select.Option value="UNRESOLVED">
          {"Close without Answer" |> ste}
        </Select.Option>,
        <Select.Option value="COMMENT"> {"Comment" |> ste} </Select.Option>,
      |],
    );

  <Select
    value={state.competitorType}
    onChange={e => send(UpdateCompetitorType(e))}>
    {options' |> ReasonReact.array}
  </Select>;
};

let dataTypeSelect = (~state, ~send): ReasonReact.reactElement =>
  <Select value={state.dataType} onChange={e => send(UpdateDataType(e))}>
    <Select.Option value="FLOAT_CDF"> {"Distribution" |> ste} </Select.Option>
    <Select.Option value="FLOAT_POINT"> {"Exact Value" |> ste} </Select.Option>
  </Select>;

let getIsValid = (state: state): bool =>
  switch (state.dataType) {
  | "FLOAT_CDF" => E.A.length(state.floatCdf.xs) > 1
  | "FLOAT_POINT" => E.A.length(state.floatCdf.xs) == 1
  | "PERCENTAGE_FLOAT" => true
  | "BINARY_BOOL" => true
  | "UNRESOLVABLE_RESOLUTION" => true
  | "COMMENT" => true
  };

let getDataTypeAsString =
    (
      competitorType: string,
      measurable: Primary.Measurable.t,
      dataType: option(string),
    )
    : string => {
  switch (competitorType, measurable.valueType, dataType) {
  | ("COMMENT", _, _) => "COMMENT"
  | ("UNRESOLVED", _, _) => "UNRESOLVABLE_RESOLUTION"
  | ("OBJECTIVE", `PERCENTAGE, _) => "BINARY_BOOL"
  | ("COMPETITIVE", `PERCENTAGE, _) => "PERCENTAGE_FLOAT"
  | _ => "FLOAT_CDF"
  };
};

let getValueFromState = (state: state): MeasurementValue.t =>
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
  | "PERCENTAGE_FLOAT" => `Percentage(state.percentage)
  | "BINARY_BOOL" => `Binary(state.binary)
  | "UNRESOLVABLE_RESOLUTION" =>
    `UnresolvableResolution(
      state.unresolvableResolution
      |> MeasurementValue.UnresolvableResolution.fromString,
    )
  | "COMMENT" =>
    `Comment(state.comment |> MeasurementValue.Comment.fromString)
  };

let getCompetitorTypeFromString = (str: string): Types.competitorType =>
  switch (str) {
  | "COMPETITIVE" => `COMPETITIVE
  | "OBJECTIVE" => `OBJECTIVE
  | "UNRESOLVED" => `UNRESOLVED
  | "COMMENT" => `COMMENT
  | _ => `OBJECTIVE
  };

let mainBlock =
    (
      ~state: state,
      ~isCreator: bool,
      ~send,
      ~onSubmit,
      ~measurable: Primary.Measurable.t,
    )
    : ReasonReact.reactElement => {
  let isValid = getIsValid(state);

  let getDataTypeSelect: ReasonReact.reactElement =
    switch (state.competitorType, measurable.valueType) {
    | ("OBJECTIVE", `FLOAT | `DATE) =>
      <div className=Styles.select> {dataTypeSelect(~state, ~send)} </div>
    | _ => ReasonReact.null
    };

  let valueInput: ReasonReact.reactElement =
    switch (state.dataType) {
    | "FLOAT_CDF"
    | "FLOAT_POINT" =>
      <>
        {state.competitorType == "OBJECTIVE"
           ? ReasonReact.null
           : <h4 className=Styles.label>
               {"Prediction (Distribution)" |> ste}
             </h4>}
        {state.hasLimitError
           ? <FC__Alert type_=`warning>
               {"Warning: Foretold does not currently support ranges of this width, due to smoothing limitations."
                |> ste}
             </FC__Alert>
           : ReasonReact.null}
        <GuesstimateInput
          focusOnRender=true
          sampleCount=30000
          onUpdate={event =>
            {let (ys, xs, hasLimitError) = event
             let asGroup: FloatCdf.t = {xs, ys}
             send(UpdateHasLimitError(hasLimitError))
             send(UpdateFloatPdf(asGroup))}
            |> ignore
          }
          onChange={text => send(UpdateValueText(text))}
        />
      </>

    | "BINARY_BOOL" =>
      <Select
        value={state.binary |> E.Bool.toString}
        onChange={e => send(UpdateBinary(e |> E.Bool.fromString))}>
        <Select.Option value="TRUE"> {"True" |> ste} </Select.Option>
        <Select.Option value="FALSE"> {"False" |> ste} </Select.Option>
      </Select>

    | "PERCENTAGE_FLOAT" =>
      <>
        <h4 className=Styles.label>
          {"Predicted Percentage Chance" |> ste}
        </h4>
        <Slider
          min=0.
          max=100.
          value={state.percentage}
          tipFormatter={(v: string) => v ++ "%"}
          step=1.
          onChange={(value: float) => send(UpdatePercentage(value))}
        />
        <InputNumber
          formatter={v => v ++ "%"}
          parser={v => Js.String.replace("%", "", v)}
          min=0.
          max=100.
          value={state.percentage}
          step=1.
          onChange={(value: float) =>
            if (value > (-0.001)) {
              // This is to fix a bug. The value could actually be undefined,
              // but the antd lib can't handle this.
              send(
                UpdatePercentage(value),
              );
            }
          }
        />
      </>

    | "UNRESOLVABLE_RESOLUTION" =>
      <Select
        value={state.unresolvableResolution}
        onChange={e => send(UpdateUnresolvableResolution(e))}>
        <Select.Option value="AMBIGUOUS"> {"Ambiguous" |> ste} </Select.Option>
        <Select.Option value="FALSE_CONDITIONAL">
          {"False Conditional" |> ste}
        </Select.Option>
        <Select.Option value="OTHER"> {"Other" |> ste} </Select.Option>
        <Select.Option value="RESULT_NOT_AVAILABLE">
          {"Result Not Available" |> ste}
        </Select.Option>
      </Select>

    | "COMMENT" =>
      <Select value={state.comment} onChange={e => send(UpdateComment(e))}>
        <Select.Option value="GENERIC"> {"Generic" |> ste} </Select.Option>
        <Select.Option value="QUESTION_FEEDBACK">
          {"Question Feedback" |> ste}
        </Select.Option>
        <Select.Option value="UPDATE"> {"Update" |> ste} </Select.Option>
      </Select>

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
      getDataTypeSelect
      <div className=Styles.inputBox> valueInput </div>
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

let component = ReasonReact.reducerComponent("CdfInput");

let make =
    (
      ~data: Mutations.MeasurementCreate.Mutation.renderPropObj,
      ~onUpdate=_ => (),
      ~isCreator=false,
      ~onSubmit=_ => (),
      ~measurable: Primary.Measurable.t,
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
      // Values
      floatCdf: FloatCdf.empty,
      percentage: 0.,
      binary: true,
      unresolvableResolution: "AMBIGUOUS",
      comment: "GENERIC",

      // OBJECTIVE, COMPETITIVE, AGGREGATION (not used here), UNRESOLVED
      competitorType: competitorTypeInitValue,
      // Used to transform Form Data Type to Measurement Type
      dataType:
        getDataTypeAsString(competitorTypeInitValue, measurable, None),

      // Strings
      description: "",
      valueText: "",

      // Form State Only
      hasLimitError: false,
    };
  },

  reducer: (action, state) =>
    switch (action) {
    | UpdateFloatPdf((floatCdf: FloatCdf.t)) =>
      onUpdate(floatCdf);
      ReasonReact.Update({...state, floatCdf});

    | UpdateHasLimitError((hasLimitError: bool)) =>
      ReasonReact.Update({...state, hasLimitError})

    | UpdateCompetitorType(competitorType) =>
      let dataType =
        getDataTypeAsString(
          competitorType,
          measurable,
          Some(state.dataType),
        );
      ReasonReact.Update({...state, competitorType, dataType});

    | UpdateDataType((dataType: string)) =>
      ReasonReact.Update({...state, dataType})

    | UpdateUnresolvableResolution((unresolvableResolution: string)) =>
      ReasonReact.Update({...state, unresolvableResolution})

    | UpdateComment((comment: string)) =>
      ReasonReact.Update({...state, comment})

    | UpdateBinary((binary: bool)) => ReasonReact.Update({...state, binary})

    | UpdatePercentage((percentage: float)) =>
      ReasonReact.Update({...state, percentage})

    | UpdateDescription((description: string)) =>
      ReasonReact.Update({...state, description})

    | UpdateValueText((valueText: string)) =>
      ReasonReact.Update({...state, valueText})
    },

  render: ({state, send}) => {
    let onSubmit = () => {
      let value = getValueFromState(state);

      onSubmit((
        value,
        getCompetitorTypeFromString(state.competitorType),
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
       | Data(_) => "Form submitted successfully." |> ste |> E.React.inH2
       | NotCalled =>
         mainBlock(~state, ~isCreator, ~send, ~onSubmit, ~measurable)
       }}
    </Style.BorderedBox>;
  },
};