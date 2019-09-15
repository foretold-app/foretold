[@genType]
type resultToJs('a) = {
  value: 'a,
  error: string,
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
  |> Belt.Result.map(_, e =>
       PredictionResolutionGroupMeasures.pointScore(
         ~marketType=`MarketScore,
         ~scoringCombination=e,
         (),
       )
     );
};

[@genType]
type item = {
  measurement: Js.Json.t,
  time: float,
};

[@genType]
let toM = (item: item) =>
  MeasurementValueJson.toMeasurementValue(item.measurement)
  ->Belt.Result.map(e =>
      PredictionResolutionOverTime.MeasurementWithTime.make(
        ~measurementValue=e,
        ~time=item.time,
      )
    );

[@genType]
let itemArray = (items: array(item)) => {
  items |> Array.map(toM) |> E.Result.arrayFlatten;
};

[@genType]
let getScoreOverTime =
    (
      agentPredictions: array(item),
      marketPredictions: array(item),
      resolution: item,
    ) => {
  (
    switch (
      itemArray(agentPredictions),
      itemArray(marketPredictions),
      toM(resolution),
    ) {
    | (Ok(agentPredictions), Ok(marketPredictions), Ok(resolution)) =>
      PredictionResolutionOverTime.fromMeasurementCombination(
        ~agentPredictions,
        ~marketPredictions=Some(marketPredictions),
        ~resolution,
      )
      |> Rationale.Result.bind(_, e =>
           PredictionResolutionOverTimeMeasures.pointScoreIntegral(
             ~scoringCombination=e,
             (),
           )
         )
    | (Error(a), _, _) => Error(a |> Js.Array.joinWith(", "))
    | (_, Error(a), _) => Error(a |> Js.Array.joinWith(", "))
    | (_, _, Error(a)) => Error(a)
    }
  )
  |> JsResult.fromResult;
};