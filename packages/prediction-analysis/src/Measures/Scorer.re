type marketScoreType =
  | MarketScore
  | NonMarketScore;

let scorePointCombination =
    (
      ~marketType: marketScoreType=MarketScore,
      ~sampleCount=DefaultParams.Cdf.maxCalculationLength,
      ~scoringCombination: PredictionResolutionGroup.t,
      (),
    ) =>
  switch (marketType) {
  | MarketScore =>
    scoringCombination
    |> PredictionResolutionGroup.toWithMarket
    |> Rationale.Result.ofOption("Needs market score")
    |> Rationale.Result.bind(
         _,
         LogScoringRule.runMarketScore(
           ~scoringCombination=_,
           ~sampleCount,
           (),
         ),
       )
  | NonMarketScore =>
    LogScoringRule.runNonmarketScore(~scoringCombination, ~sampleCount, ())
  };

let scoredIntegralOverTime =
    (
      ~marketType: marketScoreType=MarketScore,
      ~sampleCount=DefaultParams.Cdf.maxCalculationLength,
      ~scoringCombination: PredictionResolutionOverTime.t,
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