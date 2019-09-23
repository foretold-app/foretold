type marketScoreType = [ | `MarketScore | `NonMarketScore];

let pointScoreDistribution =
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
  |> StartAtDistribution.transposeResult;
};

let averagePointScore =
    (
      ~marketType: marketScoreType=`MarketScore,
      ~sampleCount=DefaultParams.Cdf.maxCalculationLength,
      ~scoringCombination: PredictionResolutionOverTime.t,
      ~lowestTimeForAverage=?,
      (),
    ) => {
  pointScoreDistribution(~marketType, ~sampleCount, ~scoringCombination, ())
  |> Belt.Result.flatMap(_, t =>
       StartAtDistribution.average(~t, ~lowestTime=?lowestTimeForAverage, ())
     );
};