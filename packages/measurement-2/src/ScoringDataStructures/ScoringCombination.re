module ValidScoringCombination = {
  open MeasurementValue;

  type combination('a, 'b) = {
    agentPrediction: 'a,
    marketPrediction: option('a),
    resolution: 'b,
  };

  type cdfCdf = combination(Cdf.t, Cdf.t);
  type cdfFloat = combination(Cdf.t, float);
  type percentagePercentage = combination(Percentage.t, Percentage.t);

  type t = [
    | `CdfCdf(cdfCdf)
    | `CdfFloat(cdfFloat)
    | `PercentagePercentage(percentagePercentage)
  ];

  let marketPredictionIsSome = (m: t) =>
    switch (m) {
    | `CdfCdf({marketPrediction: None}) => false
    | `CdfFloat({marketPrediction: None}) => false
    | `PercentagePercentage({marketPrediction: None}) => false
    | _ => true
    };
};

module ScoringCombinationInput = {
  open MeasurementValue;
  type marketScoreType =
    | MarketScore
    | NonMarketScore;

  type t = {
    agentPrediction: MeasurementValue.t,
    marketPrediction: option(MeasurementValue.t),
    resolution: MeasurementValue.t,
    marketScoreType,
  };

  let make =
      (
        ~agentPrediction,
        ~marketPrediction=None,
        ~resolution,
        ~marketScoreType=NonMarketScore,
        (),
      ) => {
    agentPrediction,
    marketPrediction,
    resolution,
    marketScoreType,
  };

  let toValidScoringCombination =
      ({agentPrediction, resolution, marketPrediction, marketScoreType}: t)
      : Belt.Result.t(ValidScoringCombination.t, string) => {
    let measurements: Belt.Result.t(ValidScoringCombination.t, string) =
      switch (marketScoreType, agentPrediction, marketPrediction, resolution) {
      | (
          MarketScore,
          `Cdf(agentPrediction),
          Some(`Cdf(marketPrediction)),
          `Cdf(resolution),
        ) =>
        Ok(
          `CdfCdf({
            agentPrediction,
            marketPrediction: Some(marketPrediction),
            resolution,
          }),
        )
      | (
          MarketScore,
          `Cdf(agentPrediction),
          Some(`Cdf(marketPrediction)),
          `Float(resolution),
        ) =>
        Ok(
          `CdfFloat({
            agentPrediction,
            marketPrediction: Some(marketPrediction),
            resolution,
          }),
        )
      | (
          MarketScore,
          `Percentage(agentPrediction),
          Some(`Percentage(marketPrediction)),
          `Percentage(resolution),
        ) =>
        Ok(
          `PercentagePercentage({
            agentPrediction,
            marketPrediction: Some(marketPrediction),
            resolution,
          }),
        )
      | (NonMarketScore, `Cdf(agentPrediction), None, `Cdf(resolution)) =>
        Ok(`CdfCdf({agentPrediction, marketPrediction: None, resolution}))
      | (NonMarketScore, `Cdf(agentPrediction), None, `Float(resolution)) =>
        Ok(`CdfFloat({agentPrediction, marketPrediction: None, resolution}))
      | (
          NonMarketScore,
          `Percentage(agentPrediction),
          None,
          `Percentage(resolution),
        ) =>
        Ok(
          `PercentagePercentage({
            agentPrediction,
            marketPrediction: None,
            resolution,
          }),
        )
      | _ =>
        Error(
          "#toValidScoringCombination error: Scoring Combination Not Valid.",
        )
      };
    measurements;
  };
};