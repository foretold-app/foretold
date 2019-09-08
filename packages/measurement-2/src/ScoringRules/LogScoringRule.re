open Belt.Result;
let log2Error = Js.Math.log2;

let marketCdfCdf =
    (
      {agentPrediction, marketPrediction, resolution}: ScoringCombination.ScoringCombinationMeasurements.cdfCdf,
      sampleCount: int,
    ) =>
  Ok(3.0);

let marketCdfFloat =
    (
      {agentPrediction, marketPrediction, resolution}: ScoringCombination.ScoringCombinationMeasurements.cdfFloat,
    ) =>
  Ok(3.0);

let marketPercentagePercentage =
    (
      {agentPrediction, marketPrediction, resolution}: ScoringCombination.ScoringCombinationMeasurements.percentagePercentage,
    ) =>
  switch (marketPrediction) {
  | None => Error("R")
  | Some(marketPrediction) =>
    Ok(
      //   TODO: Handle cases where market is 0 or 1
      MeasurementValue.Percentage.(
        {
          let positive =
            resolution *. log2Error(agentPrediction /. marketPrediction);
          let negative =
            inverse(resolution)
            *. log2Error(
                 inverse(agentPrediction) /. inverse(marketPrediction),
               );
          positive +. negative;
        }
      ),
    )
  };

let nonMarketCdfCdf =
    (
      {agentPrediction, resolution}: ScoringCombination.ScoringCombinationMeasurements.cdfCdf,
      sampleCount: int,
    ) =>
  Ok(3.0);

let nonMarketCdfFloat =
    (
      {agentPrediction, resolution}: ScoringCombination.ScoringCombinationMeasurements.cdfFloat,
    ) =>
  Ok(3.0);

let nonMarketPercentagePercentage =
    (
      {agentPrediction, resolution}: ScoringCombination.ScoringCombinationMeasurements.percentagePercentage,
    ) =>
  Ok(
    MeasurementValue.Percentage.(
      {
        let positive = resolution *. log2Error(agentPrediction /. resolution);
        let negative =
          inverse(resolution)
          *. log2Error(inverse(agentPrediction) /. inverse(resolution));
        positive +. negative;
      }
    ),
  );

type marketType =
  | Market
  | Nonmarket;

let score =
    (
      ~marketType: marketType=Market,
      ~scoringCombination: ScoringCombination.ValidScoringCombination.t,
    ) => {
  let marketPredictionExists =
    ScoringCombination.ScoringCombinationMeasurements.marketPredictionIsSome(
      scoringCombination.measurements,
    );
  switch (marketType, marketPredictionExists, scoringCombination.measurements) {
  | (Market, false, _) => Error("Market prediction needed")
  | (Market, true, `CdfCdf(v)) =>
    marketCdfCdf(v, scoringCombination.config.sampleCount)
  | (Market, true, `CdfFloat(v)) => marketCdfFloat(v)
  | (Market, true, `PercentagePercentage(v)) => marketPercentagePercentage(v)
  | (Nonmarket, _, `CdfCdf(v)) =>
    nonMarketCdfCdf(v, scoringCombination.config.sampleCount)
  | (Nonmarket, _, `CdfFloat(v)) => nonMarketCdfFloat(v)
  | (Nonmarket, _, `PercentagePercentage(v)) =>
    nonMarketPercentagePercentage(v)
  };
};