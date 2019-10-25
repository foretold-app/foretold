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

module CompetitorTypeSelect = {
  [@react.component]
  let make =
      (~isOwner: bool, ~state: state, ~send, ~measurable: Types.measurable) => {
    let options =
      Primary.CompetitorType.availableSelections(
        ~isOwner,
        ~state=measurable.state,
      );

    ReasonReact.null;
  };
};

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
    (~state, ~send, ~bots: array(Types.bot), ~loggedUser: Types.user) => {
  let name =
    loggedUser.agent
    |> E.O.fmap((agent: Types.agent) => agent.name |> E.O.default("Me"))
    |> E.O.default("Me");
  <>
    <div className=Styles.inputBox>
      <h4 className=Styles.label> {"Do this as:" |> ste} </h4>
    </div>
    ReasonReact.null
  </>;
};

module ValueInput = {
  let floatPoint = (measurable: Types.measurable, send) => ReasonReact.null;

  let boolean = (binaryValue, send) => ReasonReact.null;

  let percentage = (percentageValue, send) =>
    <> ReasonReact.null ReasonReact.null </>;

  let unresolvable = (unresolvableResolution, send) => ReasonReact.null;

  let comment = (comment, send) => ReasonReact.null;
};

module MainBlock = {
  [@react.component]
  let make =
      (
        ~state: state,
        ~isCreator: bool,
        ~send,
        ~onSubmit,
        ~measurable: Types.measurable,
        ~bots: option(array(Types.bot)),
        ~loggedUser: Types.user,
      ) => {
    let isValid = getIsValid(state);

    let getBotSelect =
      switch (bots) {
      | Some([||])
      | None => ReasonReact.null
      | Some(bots) => botsSelect(~state, ~send, ~bots, ~loggedUser)
      };
    open Style.Grid;
    let valueInput =
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
          {state.hasLimitError ? ReasonReact.null : ReasonReact.null}
          ReasonReact.null
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
        {E.A.length(state.floatCdf.xs) > 1 ? ReasonReact.null : <div />}
      </div>
      <div className=Styles.inputSection>
        <div className=Styles.select> ReasonReact.null </div>
        {switch (state.competitorType, measurable.valueType) {
         | ("OBJECTIVE", `FLOAT | `DATE) =>
           <div className=Styles.select> ReasonReact.null </div>
         | _ => ReasonReact.null
         }}
        valueInput
        ReasonReact.null
        {Primary.User.show(loggedUser, getBotSelect)}
        <div className=Styles.submitButton> ReasonReact.null </div>
      </div>
    </div>;
  };
};

//let component = ReasonReact.reducerComponent("CdfInput");
//let make =
//    (
//      ~data: MeasurementCreate.Mutation.renderPropObj,
//      ~onUpdate=_ => (),
//      ~isCreator=false,
//      ~onSubmit=_ => (),
//      ~measurable: Types.measurable,
//      ~bots: option(array(Types.bot)),
//      ~loggedUser: Types.user,
//      _children,
//    ) => {
//  ...component,
//
//  initialState: () => {
//    let competitorTypeInitValue =
//      switch (measurable.state) {
//      | Some(`JUDGED) => "OBJECTIVE"
//      | _ => "COMPETITIVE"
//      };
//
//    {
//      // Values
//      floatCdf: FloatCdf.empty,
//      percentage: 0.,
//      binary: true,
//      unresolvableResolution: "AMBIGUOUS",
//      comment: "GENERIC",
//
//      // OBJECTIVE, COMPETITIVE, AGGREGATION (not used here), UNRESOLVED
//      competitorType: competitorTypeInitValue,
//      // Used to transform Form Data Type to Measurement Type
//      dataType:
//        getDataTypeAsString(competitorTypeInitValue, measurable, None),
//
//      // Strings
//      description: "",
//      valueText: "",
//
//      // Form State Only
//      hasLimitError: false,
//      asAgent: "",
//    };
//  },
//
//  reducer: (action, state) =>
//    switch (action) {
//    | UpdateFloatPdf((floatCdf: FloatCdf.t)) =>
//      onUpdate(floatCdf);
//      ReasonReact.Update({...state, floatCdf});
//
//    | UpdateHasLimitError((hasLimitError: bool)) =>
//      ReasonReact.Update({...state, hasLimitError})
//
//    | UpdateCompetitorType(competitorType) =>
//      let dataType =
//        getDataTypeAsString(
//          competitorType,
//          measurable,
//          Some(state.dataType),
//        );
//      ReasonReact.Update({...state, competitorType, dataType});
//
//    | UpdateDataType((dataType: string)) =>
//      ReasonReact.Update({...state, dataType})
//
//    | UpdateUnresolvableResolution((unresolvableResolution: string)) =>
//      ReasonReact.Update({...state, unresolvableResolution})
//
//    | UpdateComment((comment: string)) =>
//      ReasonReact.Update({...state, comment})
//
//    | UpdateBinary((binary: bool)) => ReasonReact.Update({...state, binary})
//
//    | UpdatePercentage((percentage: float)) =>
//      ReasonReact.Update({...state, percentage})
//
//    | UpdateDescription((description: string)) =>
//      ReasonReact.Update({...state, description})
//
//    | UpdateValueText((valueText: string)) =>
//      ReasonReact.Update({...state, valueText})
//
//    | UpdateAsAgent((asAgent: string)) =>
//      ReasonReact.Update({...state, asAgent})
//    },
//
//  render: ({state, send}) => {
//    let onSubmit = () => {
//      let value = getValueFromState(state);
//
//      onSubmit((
//        value,
//        getCompetitorTypeFromString(state.competitorType),
//        state.description,
//        state.valueText,
//        state.asAgent,
//      ));
//
//      ();
//    };
//
//    let block =
//      mainBlock(
//        ~state,
//        ~isCreator,
//        ~send,
//        ~onSubmit,
//        ~measurable,
//        ~bots,
//        ~loggedUser,
//      );
//
//    <SStyle.BorderedBox>
//      {switch (data.result) {
//       | Loading => "Loading" |> ste
//       | Error(e) => <> {"Error: " ++ e##message |> ste} block </>
//       | Data(_) => "Form submitted successfully." |> ste |> E.React.inH2
//       | NotCalled => block
//       }}
//    </SStyle.BorderedBox>;
//  },
//};

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
  let send = (action: action) => ();

  let competitorTypeInitValue =
    switch (measurable.state) {
    | Some(`JUDGED) => "OBJECTIVE"
    | _ => "COMPETITIVE"
    };

  let state = {
    // Values
    floatCdf: FloatCdf.empty,
    percentage: 0.,
    binary: true,
    unresolvableResolution: "AMBIGUOUS",
    comment: "GENERIC",

    // OBJECTIVE, COMPETITIVE, AGGREGATION (not used here), UNRESOLVED
    competitorType: competitorTypeInitValue,
    // Used to transform Form Data Type to Measurement Type
    dataType: getDataTypeAsString(competitorTypeInitValue, measurable, None),

    // Strings
    description: "",
    valueText: "",

    // Form State Only
    hasLimitError: false,
    asAgent: "",
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

  ReasonReact.null;
};
