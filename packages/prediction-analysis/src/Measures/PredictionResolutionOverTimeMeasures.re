type marketScoreType = [ | `MarketScore | `NonMarketScore];

let averagePointScore =
    (
      ~marketType: marketScoreType=`MarketScore,
      ~sampleCount=DefaultParams.Cdf.maxCalculationLength,
      ~scoringCombination: PredictionResolutionOverTime.t,
      ~lowestTimeForAverage=?,
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
  |> Belt.Result.flatMap(_, t =>
       StartAtDistribution.average(~t, ~lowestTime=?lowestTimeForAverage, ())
     );
};