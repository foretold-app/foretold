[@bs.config {jsx: 3}];

open E;
open Utils;

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
  // another
  asAgent: string,
};

let tutorialSource = "
Use this to quickly enter probability distributions. You can experiment with interactive and more complex examples [here](https://observablehq.com/@oagr/foretold-inputs).\n
### Simple Examples \n
`10 to 100`\n
A lognormal distribution with a 90% confidence interval between 10 and 100.\n
`10M to 100M`\n
The same as above, but between 10 Million and 100 Million. \n
`=normal(5,2)`\n
A normal distribution with a mean of 5 and a standard deviation of 2.";

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
  | UpdateValueText(string)
  | UpdateAsAgent(string);

module Styles = {
  open Css;
  let form =
    style([display(`flex), flexDirection(`row), width(`percent(100.))]);
  let chartSection = style([flex(`num(1.))]);
  let inputSection =
    style([flex(`num(1.)), marginTop(px(10)), marginRight(px(5))]);
  let inputBox = style([]);
  let submitButton = style([marginTop(px(20))]);
  let select = style([marginBottom(px(7))]);
  let label = style([color(hex("888"))]);
  let fullWidth = style([minWidth(`percent(100.))]);
};

let competitorTypeSelect =
    (~isOwner: bool, ~state: state, ~send, ~measurable: Types.measurable)
    : ReasonReact.reactElement => {
  let options =
    Primary.CompetitorType.availableSelections(
      ~isOwner,
      ~state=measurable.state,
    );

  <Antd.Select
    value={state.competitorType}
    className=Styles.fullWidth
    onChange={e => send(UpdateCompetitorType(e))}>
    {options |> ReasonReact.array}
  </Antd.Select>;
};

let dataTypeSelect = (~state, ~send): ReasonReact.reactElement =>
  <Antd.Select
    value={state.dataType}
    onChange={e => send(UpdateDataType(e))}
    className=Styles.fullWidth>
    <Antd.Select.Option value="FLOAT_CDF">
      {"Distribution" |> ste}
    </Antd.Select.Option>
    <Antd.Select.Option value="FLOAT_POINT">
      {"Exact Value" |> ste}
    </Antd.Select.Option>
  </Antd.Select>;

let getIsValid = (state: state): bool => {
  switch (state.dataType) {
  | "FLOAT_CDF" => E.A.length(state.floatCdf.xs) > 1
  | "FLOAT_POINT" => E.A.length(state.floatCdf.xs) == 1
  | "PERCENTAGE_FLOAT" => true
  | "BINARY_BOOL" => true
  | "UNRESOLVABLE_RESOLUTION" => true
  | "COMMENT" => true
  | _ => true
  };
};

let getDataTypeAsString =
    (
      competitorType: string,
      measurable: Types.measurable,
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

let botsSelect =
    (~state, ~send, ~bots: array(Types.bot), ~loggedUser: Types.user)
    : ReasonReact.reactElement => {
  let name =
    loggedUser.agent
    |> E.O.fmap((agent: Types.agent) => agent.name |> E.O.default("Me"))
    |> E.O.default("Me");
  <>
    <div className=Styles.inputBox>
      <h4 className=Styles.label> {"Do this as:" |> ste} </h4>
    </div>
    <Antd.Select
      value={state.asAgent}
      onChange={e => send(UpdateAsAgent(e))}
      className=Styles.fullWidth>
      <Antd.Select.Option value=""> {name |> ste} </Antd.Select.Option>
      {bots
       |> Array.map((bot: Types.bot) =>
            <Antd.Select.Option
              value={
                bot.agent
                |> E.O.fmap((agent: Types.agent) => agent.id)
                |> E.O.default("")
              }>
              {bot.name |> E.O.default(bot.id) |> ste}
            </Antd.Select.Option>
          )
       |> ReasonReact.array}
    </Antd.Select>
  </>;
};

module ValueInput = {
  let floatPoint = (measurable: Types.measurable, send) =>
    <GuesstimateInput
      focusOnRender=true
      sampleCount=30000
      min={measurable.min}
      max={measurable.max}
      onUpdate={event =>
        {
          let (ys, xs, hasLimitError) = event;
          let asGroup: FloatCdf.t = {xs, ys};
          send(UpdateHasLimitError(hasLimitError));
          send(UpdateFloatPdf(asGroup));
        }
        |> ignore
      }
      onChange={text => send(UpdateValueText(text))}
    />;

  let boolean = (binaryValue, send) =>
    <Antd.Select
      value={binaryValue |> E.Bool.toString}
      className=Styles.fullWidth
      onChange={e => send(UpdateBinary(e |> E.Bool.fromString))}>
      <Antd.Select.Option value="TRUE"> {"True" |> ste} </Antd.Select.Option>
      <Antd.Select.Option value="FALSE"> {"False" |> ste} </Antd.Select.Option>
    </Antd.Select>;

  let percentage = (percentageValue, send) =>
    <>
      <Slider
        min=0.
        max=100.
        value={percentageValue |> int_of_float}
        tipFormatter={(v: string) => v ++ "%"}
        step=1.
        onChange={(value: float) => send(UpdatePercentage(value))}
      />
      <InputNumber
        formatter={v => string_of_int(v) ++ "%"}
        parser={v => Js.String.replace("%", "", v) |> int_of_string}
        min=0
        max=100
        value={percentageValue |> int_of_float}
        step=1
        onChange={(value: int) => {
          let value = float_of_int(value);
          if (value > (-0.001)) {
            // This is to fix a bug. The value could actually be undefined,
            // but the antd lib can't handle this.
            send(
              UpdatePercentage(value),
            );
          };
        }}
      />
    </>;

  let unresolvable = (unresolvableResolution, send) =>
    <Antd.Select
      value=unresolvableResolution
      className=Styles.fullWidth
      onChange={e => send(UpdateUnresolvableResolution(e))}>
      <Antd.Select.Option value="AMBIGUOUS">
        {"Result Ambiguous" |> ste}
      </Antd.Select.Option>
      <Antd.Select.Option value="RESULT_NOT_AVAILABLE">
        {"Result Not Available" |> ste}
      </Antd.Select.Option>
      <Antd.Select.Option value="FALSE_CONDITIONAL">
        {"Necessary Conditional was False" |> ste}
      </Antd.Select.Option>
      <Antd.Select.Option value="OTHER"> {"Other" |> ste} </Antd.Select.Option>
    </Antd.Select>;

  let comment = (comment, send) =>
    <Antd.Select
      value=comment
      onChange={e => send(UpdateComment(e))}
      className=Styles.fullWidth>
      <Antd.Select.Option value="GENERIC">
        {"Generic Comment" |> ste}
      </Antd.Select.Option>
      <Antd.Select.Option value="QUESTION_FEEDBACK">
        {"Question Feedback" |> ste}
      </Antd.Select.Option>
      <Antd.Select.Option value="UPDATE">
        {"Relevant Information Update" |> ste}
      </Antd.Select.Option>
    </Antd.Select>;
};

let mainBlock =
    (
      ~state: state,
      ~isCreator: bool,
      ~send,
      ~onSubmit,
      ~measurable: Types.measurable,
      ~bots: option(array(Types.bot)),
      ~loggedUser: Types.user,
    )
    : ReasonReact.reactElement => {
  let isValid = getIsValid(state);

  let getDataTypeSelect: ReasonReact.reactElement =
    switch (state.competitorType, measurable.valueType) {
    | ("OBJECTIVE", `FLOAT | `DATE) =>
      <div className=Styles.select> {dataTypeSelect(~state, ~send)} </div>
    | _ => ReasonReact.null
    };

  let getBotSelect: ReasonReact.reactElement =
    switch (bots) {
    | Some([||])
    | None => ReasonReact.null
    | Some(bots) => botsSelect(~state, ~send, ~bots, ~loggedUser)
    };
  open Style.Grid;
  let valueInput: ReasonReact.reactElement =
    switch (state.dataType) {
    | "FLOAT_CDF"
    | "FLOAT_POINT" =>
      <>
        <h4 className=Styles.label>
          {(
             state.competitorType == "OBJECTIVE"
               ? "Result" : "Prediction (Distribution)"
           )
           |> ste}
        </h4>
        {Primary.Measurable.toMinMaxDescription(measurable)
         |> E.O.React.fmapOrNull(r => <p> {r |> ste} </p>)}
        {state.hasLimitError
           ? <FC__Alert type_=`warning>
               {"Warning: Foretold does not currently support ranges of this width, due to smoothing limitations."
                |> ste}
             </FC__Alert>
           : ReasonReact.null}
        <Div>
          <Div
            float=`left
            styles=[
              Css.(style([width(Css.Calc.(`percent(100.0) - `em(2.2)))])),
            ]>
            {ValueInput.floatPoint(measurable, send)}
          </Div>
          <Div float=`left styles=[Css.(style([width(`em(2.2))]))]>
            <span
              className=Css.(style([float(`right), fontSize(`em(1.6))]))>
              <FC.HelpDropdown
                content=FC.HelpDropdown.{
                  headerContent: "Distribution Editor" |> ste,
                  bodyContent: <ReactMarkdown source=tutorialSource />,
                }
              />
            </span>
          </Div>
        </Div>
        <div className=Styles.inputBox>
          <h4 className=Styles.label> {"Reasoning" |> ste} </h4>
        </div>
      </>

    | "BINARY_BOOL" =>
      <>
        <h4 className=Styles.label> {"Result" |> ste} </h4>
        {ValueInput.boolean(state.binary, send)}
        <div className=Styles.inputBox>
          <h4 className=Styles.label> {"Reasoning" |> ste} </h4>
        </div>
      </>
    | "PERCENTAGE_FLOAT" =>
      <>
        <h4 className=Styles.label>
          {"Predicted Percentage Chance" |> ste}
        </h4>
        {ValueInput.percentage(state.percentage, send)}
        <div className=Styles.inputBox>
          <h4 className=Styles.label> {"Reasoning" |> ste} </h4>
        </div>
      </>

    | "UNRESOLVABLE_RESOLUTION" =>
      <>
        <div className=Styles.inputBox>
          <h4 className=Styles.label> {"Reason for closing" |> ste} </h4>
        </div>
        {ValueInput.unresolvable(state.unresolvableResolution, send)}
        <div className=Styles.inputBox>
          <h4 className=Styles.label> {"Reasoning" |> ste} </h4>
        </div>
      </>

    | "COMMENT" =>
      <>
        {Primary.User.show(
           loggedUser,
           <>
             <div className=Styles.inputBox>
               <h4 className=Styles.label> {"Comment Type" |> ste} </h4>
             </div>
             {ValueInput.comment(state.comment, send)}
           </>,
         )}
        <div className=Styles.inputBox>
          <h4 className=Styles.label> {"Comment" |> ste} </h4>
        </div>
      </>

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
      <div className=Styles.select>
        {competitorTypeSelect(~isOwner=isCreator, ~state, ~send, ~measurable)}
      </div>
      getDataTypeSelect
      valueInput
      <Antd.Input.TextArea
        value={state.description}
        onChange={event => {
          let value = ReactEvent.Form.target(event)##value;
          send(UpdateDescription(value));
        }}
      />
      {Primary.User.show(loggedUser, getBotSelect)}
      <div className=Styles.submitButton>
        <Antd.Button
          _type=`primary onClick={_ => onSubmit()} disabled={!isValid}>
          {"Submit" |> ste}
        </Antd.Button>
      </div>
    </div>
  </div>;
};

let a9 = React.useState(() => FloatCdf.empty);
let a10 = React.useState(() => 0.);
let a11 = React.useState(() => true);
let a12 = React.useState(() => "AMBIGUOUS");
let a13 = React.useState(() => "GENERIC");
//let competitorTypeInitValue =
//  switch (measurable.state) {
//  | Some(`JUDGED) => "OBJECTIVE"
//  | _ => "COMPETITIVE"
//  };
let a14 = React.useState(() => "COMPETITIVE"); // @todo 1
let a15 = React.useState(() => "FLOAT_CDF"); // @todo 2
let a16 = React.useState(() => "");
let a17 = React.useState(() => "");
let a18 = React.useState(() => false);
let a19 = React.useState(() => "");

[@react.component]
let make =
    (
      ~data: MeasurementCreate.Mutation.renderPropObj,
      ~onUpdate=_ => (),
      ~isCreator=false,
      ~onSubmit=_ => (),
      ~measurable: Types.measurable,
      ~bots: option(array(Types.bot)),
      ~loggedUser: Types.user,
    ) => {
  let (floatCdf, setFloatPdf) = a9;
  let (percentage, setPercentage) = a10;
  let (binary, setBinary) = a11;
  let (unresolvableResolution, setUnresolvableResolution) = a12;
  let (comment, setComment) = a13;
  let (competitorType, setCompetitorType) = a14;
  let (dataType, setDataType) = a15;
  let (description, setDescription) = a16;
  let (valueText, setValueText) = a17;
  let (hasLimitError, setHasLimitError) = a18;
  let (asAgent, setAsAgent) = a19;

  let state = {
    // Values
    floatCdf,
    percentage,
    binary,
    unresolvableResolution,
    comment,

    // OBJECTIVE, COMPETITIVE, AGGREGATION (not used here), UNRESOLVED
    competitorType,
    // Used to transform Form Data Type to Measurement Type
    dataType,

    // Strings
    description,
    valueText,

    // Form State Only
    hasLimitError,
    asAgent,
  };

  let send = action => {
    switch (action) {
    | UpdateFloatPdf((floatCdf: FloatCdf.t)) =>
      onUpdate(floatCdf);
      setFloatPdf(_ => floatCdf);

    | UpdateHasLimitError((hasLimitError: bool)) =>
      setHasLimitError(_ => hasLimitError)

    | UpdateCompetitorType(competitorType) =>
      let dataType =
        getDataTypeAsString(
          competitorType,
          measurable,
          Some(state.dataType),
        );
      setCompetitorType(_ => competitorType);
      setDataType(_ => dataType);

    | UpdateDataType((dataType: string)) => setDataType(_ => dataType)

    | UpdateUnresolvableResolution((unresolvableResolution: string)) =>
      setUnresolvableResolution(_ => unresolvableResolution)

    | UpdateComment((comment: string)) => setComment(_ => comment)

    | UpdateBinary((binary: bool)) => setBinary(_ => binary)

    | UpdatePercentage((percentage: float)) => setPercentage(_ => percentage)

    | UpdateDescription((description: string)) =>
      setDescription(_ => description)

    | UpdateValueText((valueText: string)) => setValueText(_ => valueText)

    | UpdateAsAgent((asAgent: string)) => setAsAgent(_ => asAgent)
    };

    ();
  };

  let onSubmit = () => {
    let value = getValueFromState(state);

    onSubmit((
      value,
      getCompetitorTypeFromString(state.competitorType),
      state.description,
      state.valueText,
      state.asAgent,
    ));

    ();
  };

  let block =
    mainBlock(
      ~state,
      ~isCreator,
      ~send,
      ~onSubmit,
      ~measurable,
      ~bots,
      ~loggedUser,
    );

  <Style.BorderedBox>
    {switch (data.result) {
     | Loading => "Loading" |> ste
     | Error(e) => <> {"Error: " ++ e##message |> ste} block </>
     | Data(_) => "Form submitted successfully." |> ste |> E.React2.inH2
     | NotCalled => block
     }}
  </Style.BorderedBox>;
};