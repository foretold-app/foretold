open Belt.Result;
let log2Error = Js.Math.log2;
let marketCdfCdf =
    (
      {agentPrediction, marketPrediction, resolution}: ScoringCombination.ValidScoringCombination.marketCdfCdf,
      sampleCount: int,
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
          resolution *. log2Error(agentPrediction /. marketPrediction);
        let negative =
          inverse(resolution)
          *. log2Error(
               inverse(agentPrediction) /. inverse(marketPrediction),
             );
        positive +. negative;
      }
    ),
  );
let nonMarketCdfCdf =
    (
      {agentPrediction, resolution}: ScoringCombination.ValidScoringCombination.nonMarketCdfCdf,
      sampleCount: int,
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
        let positive = resolution *. log2Error(agentPrediction /. resolution);
        let negative =
          inverse(resolution)
          *. log2Error(inverse(agentPrediction) /. inverse(resolution));
        positive +. negative;
      }
    ),
  );