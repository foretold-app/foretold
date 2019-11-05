type marketScoreType = [ | `MarketScore | `NonMarketScore];

let limitScore = (~max, ~min, ~score) => {
  switch (score) {
  | score when score == nan => nan
  | score when score == infinity => max
  | score when score > max => max
  | score when score == neg_infinity => min
  | score when score < (-20.) => min
  | score => score
  };
};

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
         scoreFn(~scoringCombination=`MarketScore(e), ~sampleCount, ())
       )
    |> Rationale.Result.fmap(limitScore(~max=20., ~min=-20., ~score=_))
  | `NonMarketScore =>
    scoreFn(
      ~scoringCombination=`NonMarketScore(scoringCombination),
      ~sampleCount,
      (),
    )
    |> Rationale.Result.fmap(limitScore(~max=20., ~min=-20., ~score=_))
  };
};