type marketScoreType =
  | MarketScore
  | NonMarketScore;

let scorePointCombination =
    (
      ~marketType: marketScoreType=MarketScore,
      ~scoringCombination: PredictionResolutionGroup.t,
      ~sampleCount=10000,
      (),
    ) =>
  switch (marketType) {
  | MarketScore =>
    LogScoringRule.runMarketScore(~scoringCombination, ~sampleCount, ())
  | NonMarketScore =>
    LogScoringRule.runNonmarketScore(~scoringCombination, ~sampleCount, ())
  };

let scoredIntegralOverTime =
    (
      ~marketType: marketScoreType=MarketScore,
      ~scoringCombination: PredictionResolutionOverTime.ValidScoringCombinationGroupOverTime.t,
      ~sampleCount=10000,
      (),
    ) => {
  scoringCombination.measurementGroup
  |> PredictionResolutionOverTime.MeasurementCombinationOverTime.toStartAtDistribution
  |> StartAtDistribution.map(scoringCombination =>
       scorePointCombination(
         ~marketType,
         ~sampleCount,
         ~scoringCombination,
         (),
       )
     )
  |> StartAtDistribution.transposeResult
  |> Belt.Result.flatMap(_, r =>
       r
       |> StartAtDistribution.integral
       |> Rationale.Result.ofOption("Integral Failed")
     );
};