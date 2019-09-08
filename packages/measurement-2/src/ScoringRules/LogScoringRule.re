open Belt.Result;
let logError = Js.Math.log2;
open ScoringCombination.ValidScoringCombination;
let marketCdfCdf =
    (
      {agentPrediction, marketPrediction, resolution, sampleCount}: ScoringCombination.ValidScoringCombination.marketCdfCdf,
    ) =>
  Ok(3.0);
let marketCdfFloat =
    (
      {agentPrediction, marketPrediction, resolution}: ScoringCombination.ValidScoringCombination.marketCdfFloat,
    ) =>
  Ok(3.0);
let marketPercentagePercentage =
    (
      {agentPrediction, marketPrediction, resolution}: ScoringCombination.ValidScoringCombination.marketPercentagePercentage,
    ) =>
  Ok(
    //   TODO: Handle cases where market is 0 or 1
    MeasurementValue.Percentage.(
      {
        let positive =
          resolution *. logError(agentPrediction /. marketPrediction);
        let negative =
          inverse(resolution)
          *. logError(inverse(agentPrediction) /. inverse(marketPrediction));
        positive +. negative;
      }
    ),
  );
let nonMarketCdfCdf =
    (
      {agentPrediction, resolution, sampleCount}: ScoringCombination.ValidScoringCombination.nonMarketCdfCdf,
    ) =>
  Ok(3.0);
let nonMarketCdfFloat =
    (
      {agentPrediction, resolution}: ScoringCombination.ValidScoringCombination.nonMarketCdfFloat,
    ) =>
  Ok(3.0);
let nonMarketPercentagePercentage =
    (
      {agentPrediction, resolution}: ScoringCombination.ValidScoringCombination.nonMarketPercentagePercentage,
    ) =>
  Ok(
    MeasurementValue.Percentage.(
      {
        let positive = resolution *. logError(agentPrediction /. resolution);
        let negative =
          inverse(resolution)
          *. logError(inverse(agentPrediction) /. inverse(resolution));
        positive +. negative;
      }
    ),
  );