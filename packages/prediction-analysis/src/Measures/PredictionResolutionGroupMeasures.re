type marketScoreType = [ | `MarketScore | `NonMarketScore];

let pointScore =
    (
      ~marketType: marketScoreType=`MarketScore,
      ~sampleCount=DefaultParams.Cdf.maxCalculationLength,
      ~scoringCombination: PredictionResolutionGroup.t,
      ~scoreFn=LogScoringRule.run,
      (),
    ) =>
  switch (marketType) {
  | `MarketScore =>
    scoringCombination
    |> PredictionResolutionGroup.toWithMarket
    |> Rationale.Result.ofOption("Needs market score")
    |> Rationale.Result.bind(_, e =>
         scoreFn(~scoringCombination=`MarketScore(e), ~sampleCount, ())
       )
  | `NonMarketScore =>
    scoreFn(
      ~scoringCombination=`NonMarketScore(scoringCombination),
      ~sampleCount,
      (),
    )
  };