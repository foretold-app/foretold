type marketScoreType = [ | `MarketScore | `NonMarketScore];

let scoredIntegralOverTime =
    (
      ~marketType: marketScoreType=`MarketScore,
      ~sampleCount=DefaultParams.Cdf.maxCalculationLength,
      ~scoringCombination: PredictionResolutionOverTime.t,
      (),
    ) => {
  scoringCombination
  |> PredictionResolutionOverTime.toStartAtDistribution
  |> StartAtDistribution.map(scoringCombination =>
       PredictionResolutionGroupMeasures.scorePointCombination(
         ~marketType,
         ~sampleCount,
         ~scoringCombination,
         (),
       )
     )
  |> StartAtDistribution.transposeResult
  |> Belt.Result.flatMap(_, StartAtDistribution.integral);
};