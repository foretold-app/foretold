type marketScoreType = [ | `MarketScore | `NonMarketScore];

let pointScoreIntegral =
    (
      ~marketType: marketScoreType=`MarketScore,
      ~sampleCount=DefaultParams.Cdf.maxCalculationLength,
      ~scoringCombination: PredictionResolutionOverTime.t,
      (),
    ) => {
  scoringCombination
  |> PredictionResolutionOverTime.toStartAtDistribution
  |> StartAtDistribution.map(scoringCombination =>
       PredictionResolutionGroupMeasures.pointScore(
         ~marketType,
         ~sampleCount,
         ~scoringCombination,
         (),
       )
     )
  |> StartAtDistribution.transposeResult
  |> Belt.Result.flatMap(_, StartAtDistribution.integral);
};