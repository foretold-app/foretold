open MeasurementValue;

module ValidScoringCombinationGroupOverTime = {
  type time = int;

  type measurementWithTime('a) =
    TypedMeasurementWithTime.TypedMeasurementWithTime.t('a);

  type measurementsWithTime('a) =
    TypedMeasurementWithTime.TypedMeasurementWithTime.tss('a);

  type marketCdfCdf = {
    agentPredictions: measurementsWithTime(Cdf.t),
    marketPredictions: measurementsWithTime(Cdf.t),
    resolution: measurementWithTime(Cdf.t),
    sampleCount: int,
  };
  type marketCdfFloat = {
    agentPredictions: measurementsWithTime(Cdf.t),
    marketPredictions: measurementsWithTime(Cdf.t),
    resolutions: measurementWithTime(float),
  };
  type marketPercentagePercentage = {
    agentPredictions: measurementsWithTime(Percentage.t),
    marketPredictions: measurementsWithTime(Percentage.t),
    resolution: measurementWithTime(Percentage.t),
  };
  type nonMarketCdfCdf = {
    agentPredictions: measurementsWithTime(Cdf.t),
    resolution: measurementWithTime(Cdf.t),
    sampleCount: int,
  };
  type nonMarketCdfFloat = {
    agentPredictions: measurementsWithTime(Cdf.t),
    resolution: measurementWithTime(float),
  };
  type nonMarketPercentagePercentage = {
    agentPredictions: measurementsWithTime(Percentage.t),
    resolution: measurementWithTime(Percentage.t),
  };
  type measurementGroup = [
    | `MarketCdfCdf(marketCdfCdf)
    | `MarketCdfFloat(marketCdfFloat)
    | `MarketPercentagePercentage(marketPercentagePercentage)
    | `NonMarketCdfCdf(nonMarketCdfCdf)
    | `NonMarketCdfFloat(nonMarketCdfFloat)
    | `NonMarketPercentagePercentage(nonMarketPercentagePercentage)
  ];

  type t = {
    measurementGroup,
    beginningTime: time,
  };

  let make = (t: t) => t;
};

module ScoringCombinationGroupOverTimeInput = {
  type marketScoreType =
    | MarketScore
    | NonMarketScore;

  type t = {
    agentPredictions: TypedMeasurementWithTime.MeasurementWithTime.ts,
    marketPredictions:
      option(TypedMeasurementWithTime.MeasurementWithTime.ts),
    resolution: TypedMeasurementWithTime.MeasurementWithTime.t,
    sampleCount: option(int),
    marketScoreType,
  };

  let toValidScoringCombination =
      (
        {
          marketScoreType,
          agentPredictions,
          marketPredictions,
          resolution,
          sampleCount,
        }: t,
      ) => {
    open TypedMeasurementWithTime.MeasurementWithTime;
    let typedAgentPredictions = agentPredictions |> toTypeOfFirstElement;
    let typedMarketPredictions =
      marketPredictions |> Rationale.Option.bind(_, toTypeOfFirstElement);
    switch (
      marketScoreType,
      typedAgentPredictions,
      typedMarketPredictions,
      resolution,
    ) {
    | (
        MarketScore,
        Some(`Cdf(prediction)),
        Some(`Cdf(market)),
        {measurement: `Float(a)},
      ) => 3.0
    | _ => 3.0
    };
  };
};