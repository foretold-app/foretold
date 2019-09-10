open Belt.Result;
let log2Error = Js.Math.log2;

let marketCdfCdf =
    (
      {agentPrediction, marketPrediction, resolution}: PredictionResolutionGroup.cdfCdf,
      sampleCount: int,
    ) =>
  Ok(3.0);

let marketCdfFloat =
    (
      {agentPrediction, marketPrediction, resolution}: PredictionResolutionGroup.cdfFloat,
    ) =>
  Ok(3.0);

let marketPercentagePercentage =
    (
      {agentPrediction, marketPrediction, resolution}: PredictionResolutionGroup.percentagePercentage,
    ) =>
  switch (marketPrediction) {
  | None => Error("R")
  | Some(marketPrediction) =>
    Ok(
      //   TODO: Handle cases where market is 0 or 1
      Percentage.(
        {
          let positive =
            toFloat(resolution)
            *. log2Error(
                 toFloat(agentPrediction) /. toFloat(marketPrediction),
               );
          let negative =
            toFloat(inverse(resolution))
            *. log2Error(
                 toFloat(inverse(agentPrediction))
                 /. toFloat(inverse(marketPrediction)),
               );
          positive +. negative;
        }
      ),
    )
  };

let nonMarketCdfCdf =
    (
      {agentPrediction, resolution}: PredictionResolutionGroup.cdfCdf,
      sampleCount: int,
    ) =>
  Ok(3.0);

let nonMarketCdfFloat =
    ({agentPrediction, resolution}: PredictionResolutionGroup.cdfFloat) =>
  Ok(3.0);

let nonMarketPercentagePercentage =
    (
      {agentPrediction, resolution}: PredictionResolutionGroup.percentagePercentage,
    ) =>
  Ok(
    Percentage.(
      {
        let positive =
          toFloat(resolution)
          *. log2Error(toFloat(agentPrediction) /. toFloat(resolution));
        let negative =
          toFloat(inverse(resolution))
          *. log2Error(
               toFloat(inverse(agentPrediction))
               /. toFloat(inverse(resolution)),
             );
        positive +. negative;
      }
    ),
  );

let runMarketScore =
    (~scoringCombination: PredictionResolutionGroup.t, ~sampleCount=10000, ()) => {
  let marketPredictionExists =
    PredictionResolutionGroup.marketPredictionIsSome(scoringCombination);
  switch (marketPredictionExists, scoringCombination) {
  | (false, _) => Error("Score error: market prediction needed")
  | (true, `CdfCdf(v)) => marketCdfCdf(v, sampleCount)
  | (true, `CdfFloat(v)) => marketCdfFloat(v)
  | (true, `PercentagePercentage(v)) => marketPercentagePercentage(v)
  };
};

let runNonmarketScore =
    (~scoringCombination: PredictionResolutionGroup.t, ~sampleCount=10000, ()) => {
  switch (scoringCombination) {
  | `CdfCdf(v) => nonMarketCdfCdf(v, sampleCount)
  | `CdfFloat(v) => nonMarketCdfFloat(v)
  | `PercentagePercentage(v) => nonMarketPercentagePercentage(v)
  };
};