module ScoringCombinationMeasurements = {
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

module ValidScoringCombination = {
  type config = {sampleCount: int};

  type t = {
    measurements: ScoringCombinationMeasurements.t,
    config,
  };

  let make = (~measurements, ~sampleCount=10000, ()) => {
    measurements,
    config: {
      sampleCount: sampleCount,
    },
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
    sampleCount: option(int),
    marketScoreType,
  };

  let toValidScoringCombination =
      (
        {
          agentPrediction,
          resolution,
          marketPrediction,
          sampleCount,
          marketScoreType,
        }: t,
      )
      : option(ValidScoringCombination.t) => {
    let measurements: option(ScoringCombinationMeasurements.t) =
      switch (marketScoreType, agentPrediction, marketPrediction, resolution) {
      | (
          MarketScore,
          `Cdf(agentPrediction),
          Some(`Cdf(marketPrediction)),
          `Cdf(resolution),
        ) =>
        Some(
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
        Some(
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
        Some(
          `PercentagePercentage({
            agentPrediction,
            marketPrediction: Some(marketPrediction),
            resolution,
          }),
        )
      | (NonMarketScore, `Cdf(agentPrediction), None, `Cdf(resolution)) =>
        Some(`CdfCdf({agentPrediction, marketPrediction: None, resolution}))
      | (NonMarketScore, `Cdf(agentPrediction), None, `Float(resolution)) =>
        Some(
          `CdfFloat({agentPrediction, marketPrediction: None, resolution}),
        )
      | (
          NonMarketScore,
          `Percentage(agentPrediction),
          None,
          `Percentage(resolution),
        ) =>
        Some(
          `PercentagePercentage({
            agentPrediction,
            marketPrediction: None,
            resolution,
          }),
        )
      | _ => None
      };
    measurements
    |> Belt.Option.map(_, measurements =>
         ValidScoringCombination.make(~measurements, ~sampleCount?, ())
       );
  };
};