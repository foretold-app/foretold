type marketScoreType = [ | `MarketScore | `NonMarketScore];

let limitScore = (~max, ~min, ~score) => {
  switch (score) {
  | score when score == nan => nan
  | score when score == infinity => max
  | score when score > max => max
  | score when score == neg_infinity => min
  | score when score < min => min
  | score => score
  };
};
let scoreLimiter = limitScore(~max=20.0, ~min=-20.0, ~score=_);

let pointScore =
    (
      ~marketType: marketScoreType=`MarketScore,
      ~sampleCount=DefaultParams.Cdf.maxCalculationLength,
      ~scoringCombination: PredictionResolutionGroup.t,
      ~scoreFn=LogError.predictionGroupError,
      (),
    ) => {
  switch (marketType) {
  | `MarketScore =>
    scoringCombination
    |> PredictionResolutionGroup.toWithMarket
    |> Rationale.Result.ofOption("Needs market score")
    |> Rationale.Result.bind(_, e =>
         scoreFn(
           ~scoringCombination=`MarketScore(e),
           ~sampleCount,
           ~scoreLimiter,
           (),
         )
       )
  | `NonMarketScore =>
    scoreFn(
      ~scoringCombination=`NonMarketScore(scoringCombination),
      ~sampleCount,
      ~scoreLimiter,
      (),
    )
  };
};