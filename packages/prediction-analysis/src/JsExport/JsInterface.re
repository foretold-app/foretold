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

module MarketType = {
  [@genType]
  type marketScoreType = [ | `MarketScore | `NonMarketScore];
  [@genType]
  let marketScore = `MarketScore;
  [@genType]
  let nonMarketScore = `NonMarketScore;
};

module MeasurementValueI = {
  [@genType]
  let make = (e: Js.Json.t) => e |> MeasurementValueJson.toMeasurementValue;

  [@genType]
  let differentialEntropy = (e: MeasurementValue.t) =>
    e |> MeasurementValueMeasures.differentialEntropy;
};

module PredictionResolutionGroupI = {
  [@genType]
  let make =
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
  let pointScore =
      (
        marketType: MarketType.marketScoreType,
        scoringCombination: PredictionAnalysis.PredictionResolutionGroup.t,
      ) => {
    PredictionResolutionGroupMeasures.pointScore(
      ~marketType,
      ~scoringCombination,
      (),
    );
  };
};

module PredictionResolutionOverTimeI = {
  [@genType]
  type item = {
    measurement: Js.Json.t,
    time: float,
  };

  let toMeasurementValue = (item: item) =>
    MeasurementValueJson.toMeasurementValue(item.measurement)
    ->Belt.Result.map(e =>
        PredictionResolutionOverTime.MeasurementWithTime.make(
          ~measurementValue=e,
          ~time=item.time,
        )
      );

  let itemArray = (items: array(item)) => {
    items |> Array.map(toMeasurementValue) |> E.Result.arrayFlatten;
  };

  [@genType]
  let make =
      (
        agentPredictions: array(item),
        marketPredictions: array(item),
        resolution: item,
      ) => {
    switch (
      itemArray(agentPredictions),
      itemArray(marketPredictions),
      toMeasurementValue(resolution),
    ) {
    | (Ok(agentPredictions), Ok(marketPredictions), Ok(resolution)) =>
      PredictionResolutionOverTime.fromMeasurementCombination(
        ~agentPredictions,
        ~marketPredictions=Some(marketPredictions),
        ~resolution,
      )
    | (Error(a), _, _) => Error(a |> Js.Array.joinWith(", "))
    | (_, Error(a), _) => Error(a |> Js.Array.joinWith(", "))
    | (_, _, Error(a)) => Error(a)
    };
  };

  [@genType]
  let pointScoreIntegral = (marketType: MarketType.marketScoreType, e) =>
    PredictionResolutionOverTimeMeasures.pointScoreIntegral(
      ~marketType,
      ~scoringCombination=e,
      (),
    );
};