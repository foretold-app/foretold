type marketScoreType =
  | MarketScore
  | NonMarketScore;

let scorePointCombination =
    (
      ~marketType: marketScoreType=MarketScore,
      ~scoringCombination: PredictionResolutionGroup.t,
      ~sampleCount=DefaultParams.Cdf.maxCalculationLength,
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
      ~scoringCombination: PredictionResolutionOverTime.t,
      ~sampleCount=DefaultParams.Cdf.maxCalculationLength,
      (),
    ) => {
  scoringCombination
  |> PredictionResolutionOverTime.toStartAtDistribution
  |> StartAtDistribution.map(scoringCombination =>
       scorePointCombination(
         ~marketType,
         ~sampleCount,
         ~scoringCombination,
         (),
       )
     )
  |> StartAtDistribution.transposeResult
  |> Belt.Result.flatMap(_, StartAtDistribution.integral);
};