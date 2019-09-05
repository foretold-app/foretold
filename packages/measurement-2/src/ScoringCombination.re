open MeasurementValue;

module ValidScoringCombination = {
  type marketCdfCdf = {
    agentPrediction: Cdf.t,
    marketPrediction: Cdf.t,
    resolution: Cdf.t,
    sampleCount: int,
  };
  type marketCdfFloat = {
    agentPrediction: Cdf.t,
    marketPrediction: Cdf.t,
    resolution: float,
  };
  type marketPercentagePercentage = {
    agentPrediction: Percentage.t,
    marketPrediction: Percentage.t,
    resolution: Percentage.t,
  };
  type nonMarketCdfCdf = {
    agentPrediction: Cdf.t,
    resolution: Cdf.t,
    sampleCount: int,
  };
  type nonMarketCdfFloat = {
    agentPrediction: Cdf.t,
    resolution: float,
  };
  type nonMarketPercentagePercentage = {
    agentPrediction: Percentage.t,
    resolution: Percentage.t,
  };
  type t = [
    | `MarketCdfCdf(marketCdfCdf)
    | `MarketCdfFloat(marketCdfFloat)
    | `MarketPercentagePercentage(marketPercentagePercentage)
    | `NonMarketCdfCdf(nonMarketCdfCdf)
    | `NonMarketCdfFloat(nonMarketCdfFloat)
    | `NonMarketPercentagePercentage(nonMarketPercentagePercentage)
  ];

  let make = (t: t) => t;
};

module ScoringCombinationInput = {
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
    switch (
      marketScoreType,
      agentPrediction,
      marketPrediction,
      resolution,
      sampleCount,
    ) {
    | (
        MarketScore,
        `Cdf(agentPrediction),
        Some(`Cdf(marketPrediction)),
        `Cdf(resolution),
        Some(sampleCount),
      ) =>
      Some(
        `MarketCdfCdf({
          agentPrediction,
          marketPrediction,
          resolution,
          sampleCount,
        }),
      )
    | (
        MarketScore,
        `Cdf(agentPrediction),
        Some(`Cdf(marketPrediction)),
        `Float(resolution),
        _,
      ) =>
      Some(`MarketCdfFloat({agentPrediction, marketPrediction, resolution}))
    | (
        MarketScore,
        `Percentage(agentPrediction),
        Some(`Percentage(marketPrediction)),
        `Percentage(resolution),
        _,
      ) =>
      Some(
        `MarketPercentagePercentage({
          agentPrediction,
          marketPrediction,
          resolution,
        }),
      )
    | (
        NonMarketScore,
        `Cdf(agentPrediction),
        None,
        `Cdf(resolution),
        Some(sampleCount),
      ) =>
      Some(`NonMarketCdfCdf({agentPrediction, resolution, sampleCount}))
    | (NonMarketScore, `Cdf(agentPrediction), None, `Float(resolution), _) =>
      Some(`NonMarketCdfFloat({agentPrediction, resolution}))
    | (
        NonMarketScore,
        `Percentage(agentPrediction),
        None,
        `Percentage(resolution),
        _,
      ) =>
      Some(`NonMarketPercentagePercentage({agentPrediction, resolution}))
    | _ => None
    };
  };
};