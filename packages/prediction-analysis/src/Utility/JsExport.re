[@genType]
let foo = 3;

[@genType]
type resultToJs('a) = {
  value: 'a,
  error: string,
};

type name = string;
type surname = string;
[@genType]
type person = {
  name,
  surname,
};

module JsResult = {
  [@genType]
  type t('a, 'b) = {
    data: option('a),
    error: option('b),
  };

  [@genType]
  let fromResult = (r: Belt.Result.t('a, 'b)): t('a, 'b) =>
    switch (r) {
    | Belt.Result.Ok(o) => {data: Some(o), error: None}
    | Error(e) => {data: None, error: Some(e)}
    };
};

[@genType]
let toMeasurementValue = e =>
  MeasurementValueJson.toMeasurementValue(e) |> JsResult.fromResult;

[@genType]
let toPredictionResolutionGroup =
    (
      agentPrediction: Js.Json.t,
      marketPrediction: option(Js.Json.t),
      resolution: Js.Json.t,
    ) => {
  let v = MeasurementValueJson.toMeasurementValue;
  switch (
    v(agentPrediction),
    marketPrediction |> Rationale.Option.map(v),
    v(resolution),
  ) {
  | (
      Belt.Result.Ok(agentPrediction),
      Some(Belt.Result.Ok(marketPrediction)),
      Belt.Result.Ok(resolution),
    ) =>
    PredictionResolutionGroup.fromArbitraryMeasurementValues(
      ~agentPrediction,
      ~marketPrediction=Some(marketPrediction),
      ~resolution,
      (),
    )
  | (Belt.Result.Ok(agentPrediction), None, Belt.Result.Ok(resolution)) =>
    PredictionResolutionGroup.fromArbitraryMeasurementValues(
      ~agentPrediction,
      ~marketPrediction=None,
      ~resolution,
      (),
    )
  | (Error(a), _, _) => Error(a)
  | (_, Some(Error(a)), _) => Error(a)
  | (_, _, Error(a)) => Error(a)
  };
};

[@genType]
let score = (a: Js.Json.t, m: option(Js.Json.t), r: Js.Json.t) => {
  toPredictionResolutionGroup(a, m, r)
  |> Belt.Result.map(
       _,
       e => {
         Js.log2("YO", e);
         PredictionResolutionGroupMeasures.pointScore(
           ~scoringCombination=e,
           (),
         );
       },
     )
  |> JsResult.fromResult;
};

[@genType]
let fromOption = (a): option(int) => a |> Rationale.Option.map(r => r + 3);