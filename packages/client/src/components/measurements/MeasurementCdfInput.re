open Utils;

type state = {
  // -> Measurement
  // 1
  competitorType: string,
  // --> 2
  dataType: string,
  // ---> 3, Measurement.value
  floatCdf: ForetoldComponents.Types.Dist.t,
  cdfType: string,
  percentage: float,
  binary: bool,
  unresolvableResolution: string,
  comment: string,
  // another
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
  | UpdateFloatCdf(ForetoldComponents.Types.Dist.t)
  | UpdateCdfType(string)
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
  let label =
    style([color(hex("888")), marginTop(px(5)), marginBottom(px(3))]);
  let fullWidth = style([minWidth(`percent(100.))]);
  let clear = style([clear(`both)]);
};

module CompetitorTypeSelect = {
  [@react.component]
  let make = (~state, ~send, ~measurable: Types.measurable) => {
    let options = Primary.CompetitorType.availableSelections(~measurable);

    <Antd.Select
      value={state.competitorType}
      className=Styles.fullWidth
      onChange={e => send(UpdateCompetitorType(e))}
      key={"competitor-type-select" ++ measurable.id}>
      {options |> ReasonReact.array}
    </Antd.Select>;
  };
};

module DataTypeSelect = {
  [@react.component]
  let make = (~state, ~send) =>
    <Antd.Select
      value={state.dataType}
      onChange={e => send(UpdateDataType(e))}
      className=Styles.fullWidth>
      <Antd.Select.Option value="BINARY_BOOL">
        {"Binary" |> ste}
      </Antd.Select.Option>
      <Antd.Select.Option value="PERCENTAGE_FLOAT">
        {"Predicted Percentage Chance" |> ste}
      </Antd.Select.Option>
    </Antd.Select>;
};

let getIsValid = (state): bool => {
  switch (state.dataType) {
  | "FLOAT_CDF" => E.A.length(state.floatCdf.xs) > 1
  | "FLOAT_CDF_AND_POINT" => E.A.length(state.floatCdf.xs) == 1
  | "PERCENTAGE_FLOAT" => true
  | "BINARY_BOOL" => true
  | "UNRESOLVABLE_RESOLUTION" => true
  | "COMMENT" => true
  | _ => true
  };
};

let getDataTypeAsString = (competitorType, measurable: Types.measurable) => {
  switch (competitorType, measurable.valueType) {
  | ("COMMENT", _) => "COMMENT"
  | ("UNRESOLVED", _) => "UNRESOLVABLE_RESOLUTION"
  | ("OBJECTIVE", `PERCENTAGE) => "BINARY_BOOL"
  | ("COMPETITIVE", `PERCENTAGE) => "PERCENTAGE_FLOAT"
  | ("OBJECTIVE", _) => "FLOAT_CDF_AND_POINT"
  | _ => "FLOAT_CDF"
  };
};

let getCompetitorTypeFromString = (str): Types.competitorType =>
  switch (str) {
  | "COMPETITIVE" => `COMPETITIVE
  | "OBJECTIVE" => `OBJECTIVE
  | "UNRESOLVED" => `UNRESOLVED
  | "COMMENT" => `COMMENT
  | _ => `OBJECTIVE
  };

let getValueFromState = (state): MeasurementValue.t =>
  switch (state.dataType, state.cdfType) {
  | ("FLOAT_CDF", _)
  | ("FLOAT_CDF_AND_POINT", "CDF") =>
    `FloatCdf(
      MeasurementValue.FloatCdf.fromArrays(
        state.floatCdf |> (e => (e.xs, e.ys)),
      ),
    )

  | ("FLOAT_CDF_AND_POINT", "POINT") =>
    `FloatPoint(Array.unsafe_get(state.floatCdf.xs, 0))

  | ("PERCENTAGE_FLOAT", _) => `Percentage(state.percentage)
  | ("BINARY_BOOL", _) => `Binary(state.binary)
  | ("UNRESOLVABLE_RESOLUTION", _) =>
    `UnresolvableResolution(
      state.unresolvableResolution
      |> MeasurementValue.UnresolvableResolution.fromString,
    )
  | ("COMMENT", _)
  | _ => `Comment(state.comment |> MeasurementValue.Comment.fromString)
  };

let getValueTextFromState = state =>
  switch (state.dataType) {
  | "FLOAT_CDF" => state.valueText
  | "FLOAT_CDF_AND_POINT" => state.valueText
  | _ => ""
  };

module BotsSelect = {
  [@react.component]
  let make =
      (~state, ~send, ~bots: array(Types.bot), ~loggedUser: Types.user) => {
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
        <Antd.Select.Option value="" key="bot--1">
          {name |> ste}
        </Antd.Select.Option>
        {bots
         |> E.A.fmapi((index, bot: Types.bot) =>
              <Antd.Select.Option
                key={"bot" ++ string_of_int(index)}
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
};

module ValueInput = {
  module Distributions = {
    let floatCdf = (measurable: Types.measurable, send) => {
      <Antd.Input
        onChange={e =>
          ReactEvent.Form.target(e)##value
          |> (
            r => {
              let distPlusInput = DistPlusInput.run(r);
              switch (distPlusInput) {
              | `Float(f) =>
                send(UpdateCdfType("POINT"));
                send(UpdateFloatCdf(f));
              | `Dist(f) =>
                send(UpdateCdfType("CDF"));
                send(UpdateFloatCdf(f));
              };
              send(UpdateValueText(r));
            }
          )
        }
      />;
    };
  };

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

module ValueInputMapper = {
  open Style.Grid;

  module DistributionInput = {
    [@react.component]
    let make = (~state, ~measurable, ~children) =>
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
           ? <ForetoldComponents.Alert type_=`warning>
               {"Warning: Foretold does not currently support ranges of this width, due to smoothing limitations."
                |> ste}
             </ForetoldComponents.Alert>
           : <Null />}
        <Div>
          <Div
            float=`left
            styles=[
              Css.(style([width(Css.Calc.(`percent(100.0) - `em(2.2)))])),
            ]>
            children
          </Div>
          <Div float=`left styles=[Css.(style([width(`em(2.2))]))]>
            <span
              className=Css.(style([float(`right), fontSize(`em(1.6))]))>
              <ForetoldComponents.HelpDropdown
                content=ForetoldComponents.HelpDropdown.{
                  headerContent: "Distribution Editor" |> ste,
                  bodyContent: <Markdown source=tutorialSource />,
                }
              />
            </span>
          </Div>
        </Div>
        <div className=Styles.clear />
        <div className=Styles.inputBox>
          <h4 className=Styles.label> {"Reasoning" |> ste} </h4>
        </div>
      </>;
  };

  [@react.component]
  let make = (~state, ~measurable, ~send, ~loggedUser) =>
    switch (state.dataType) {
    | "FLOAT_CDF"
    | "FLOAT_CDF_AND_POINT" =>
      <DistributionInput state measurable>
        {ValueInput.Distributions.floatCdf(measurable, send)}
      </DistributionInput>

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

    | _ => <Null />
    };
};

module Main = {
  [@react.component]
  let make =
      (
        ~state: state,
        ~send,
        ~onSubmit,
        ~measurable: Types.measurable,
        ~bots: option(array(Types.bot)),
        ~loggedUser: Types.user,
      ) => {
    let isValid = getIsValid(state);

    let dataTypeSelect =
      switch (state.competitorType, measurable.valueType) {
      | ("OBJECTIVE", `PERCENTAGE) =>
        <div className=Styles.select> <DataTypeSelect state send /> </div>
      | _ => <Null />
      };

    let botSelect =
      switch (bots) {
      | Some([||])
      | None => <Null />
      | Some(bots) => <BotsSelect state send bots loggedUser />
      };

    <div className=Styles.form>
      <div className=Styles.chartSection>
        {switch (state.dataType, state.cdfType) {
         | ("FLOAT_CDF_AND_POINT", "CDF")
         | ("FLOAT_CDF", _) when E.A.length(state.floatCdf.xs) > 1 =>
           <ForetoldComponents.CdfChart__Large
             cdf={state.floatCdf}
             width={Some(0)}
           />

         | _ => <Null />
         }}
      </div>
      <div className=Styles.inputSection>
        <div className=Styles.select>
          <CompetitorTypeSelect state send measurable />
        </div>
        dataTypeSelect
        <ValueInputMapper state measurable send loggedUser />
        <ForetoldComponents.ReAutosizeTextareaInput
          value={state.description}
          className="ant-input"
          minRows=6
          onChange={value => send(UpdateDescription(value))}
        />
        {Primary.User.show(loggedUser, botSelect)}
        <div className=Styles.submitButton>
          <Antd.Button
            _type=`primary onClick={_ => onSubmit()} disabled={!isValid}>
            {"Submit" |> ste}
          </Antd.Button>
        </div>
      </div>
    </div>;
  };
};

module MarkdownPreprocessing = {
  let addNewLines = (description): string => {
    let duplicateNewLines =
      Js.String.replaceByRe([%re "/[\\n]/g"], "\n\n", description);
    let pruneCuadrupleLines =
      Js.String.replaceByRe(
        [%re "/[\\n\\n\\n\\n]/g"],
        "\n\n",
        duplicateNewLines,
      );
    pruneCuadrupleLines;
  };
};

[@react.component]
let make =
    (
      ~data: MeasurementCreate.Mutation.renderPropObj,
      ~onUpdate=_ => (),
      ~onSubmit=_ => (),
      ~measurable: Types.measurable,
      ~bots: option(array(Types.bot)),
      ~loggedUser: Types.user,
      ~defaultValueText="",
    ) => {
  let competitorTypeInitValue =
    Primary.CompetitorType.competitorTypeInitValue(~measurable);

  let (floatCdf, setFloatCdf) =
    React.useState(() => ForetoldComponents.Types.Dist.empty);
  let (cdfType, setCdfType) = React.useState(() => "CDF");

  let (percentage, setPercentage) = React.useState(() => 50.);
  let (binary, setBinary) = React.useState(() => true);
  let (unresolvableResolution, setUnresolvableResolution) =
    React.useState(() => "AMBIGUOUS");
  let (comment, setComment) = React.useState(() => "GENERIC");
  let (competitorType, setCompetitorType) =
    React.useState(() => competitorTypeInitValue);
  let (dataType, setDataType) =
    React.useState(() =>
      getDataTypeAsString(competitorTypeInitValue, measurable)
    );
  let (description, setDescription) = React.useState(() => "");
  let (valueText, setValueText) = React.useState(() => defaultValueText);
  let (hasLimitError, setHasLimitError) = React.useState(() => false);
  let (asAgent, setAsAgent) = React.useState(() => "");

  let state = {
    // Values
    floatCdf,
    cdfType,
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
    | UpdateFloatCdf((floatCdf: ForetoldComponents.Types.Dist.t)) =>
      setFloatCdf(_ => floatCdf)

    | UpdateCdfType((cdfType: string)) => setCdfType(_ => cdfType)

    | UpdateHasLimitError((hasLimitError: bool)) =>
      setHasLimitError(_ => hasLimitError)

    | UpdateCompetitorType(competitorType) =>
      let dataType = getDataTypeAsString(competitorType, measurable);
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
    let valueText = getValueTextFromState(state);

    onSubmit((
      value,
      getCompetitorTypeFromString(state.competitorType),
      state.description,
      valueText,
      state.asAgent,
    ));

    ();
  };

  let block = <Main state send onSubmit measurable bots loggedUser />;

  <Style.BorderedBox>
    {switch (data.result) {
     | Loading => <Spin />
     | Error(e) => <> {"Error: " ++ e.message |> ste} block </>
     | Data(_) => "Form submitted successfully." |> ste |> E.React2.inH2
     | NotCalled => block
     }}
  </Style.BorderedBox>;
};