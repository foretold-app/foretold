module ValidScoringCombination = {
  open MeasurementValue;

  type marketCombination('a, 'b) = {
    agentPrediction: 'a,
    marketPrediction: 'a,
    resolution: 'b,
  };

  type nonMarketCombination('a, 'b) = {
    agentPrediction: 'a,
    resolution: 'b,
  };

  type marketCdfCdf = marketCombination(Cdf.t, Cdf.t);
  type marketCdfFloat = marketCombination(Cdf.t, float);
  type marketPercentagePercentage =
    marketCombination(Percentage.t, Percentage.t);
  type nonMarketCdfCdf = nonMarketCombination(Cdf.t, Cdf.t);
  type nonMarketCdfFloat = nonMarketCombination(Cdf.t, float);
  type nonMarketPercentagePercentage =
    nonMarketCombination(Percentage.t, Percentage.t);

  type measurements = [
    | `MarketCdfCdf(marketCdfCdf)
    | `MarketCdfFloat(marketCdfFloat)
    | `MarketPercentagePercentage(marketPercentagePercentage)
    | `NonMarketCdfCdf(nonMarketCdfCdf)
    | `NonMarketCdfFloat(nonMarketCdfFloat)
    | `NonMarketPercentagePercentage(nonMarketPercentagePercentage)
  ];

  type config = {sampleCount: int};

  type t = {
    measurements,
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
    let measurements: option(ValidScoringCombination.measurements) =
      switch (marketScoreType, agentPrediction, marketPrediction, resolution) {
      | (
          MarketScore,
          `Cdf(agentPrediction),
          Some(`Cdf(marketPrediction)),
          `Cdf(resolution),
        ) =>
        Some(`MarketCdfCdf({agentPrediction, marketPrediction, resolution}))
      | (
          MarketScore,
          `Cdf(agentPrediction),
          Some(`Cdf(marketPrediction)),
          `Float(resolution),
        ) =>
        Some(
          `MarketCdfFloat({agentPrediction, marketPrediction, resolution}),
        )
      | (
          MarketScore,
          `Percentage(agentPrediction),
          Some(`Percentage(marketPrediction)),
          `Percentage(resolution),
        ) =>
        Some(
          `MarketPercentagePercentage({
            agentPrediction,
            marketPrediction,
            resolution,
          }),
        )
      | (NonMarketScore, `Cdf(agentPrediction), None, `Cdf(resolution)) =>
        Some(`NonMarketCdfCdf({agentPrediction, resolution}))
      | (NonMarketScore, `Cdf(agentPrediction), None, `Float(resolution)) =>
        Some(`NonMarketCdfFloat({agentPrediction, resolution}))
      | (
          NonMarketScore,
          `Percentage(agentPrediction),
          None,
          `Percentage(resolution),
        ) =>
        Some(`NonMarketPercentagePercentage({agentPrediction, resolution}))
      | _ => None
      };
    measurements
    |> Belt.Option.map(_, measurements =>
         ValidScoringCombination.make(~measurements, ~sampleCount?, ())
       );
  };
};