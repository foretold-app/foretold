module ValidScoringCombinationGroupOverTime = {
  type time = int;

  type measurementWithTime('a) =
    TypedMeasurementWithTime.TypedMeasurementWithTime.t('a);

  type measurementsWithTime('a) =
    TypedMeasurementWithTime.TypedMeasurementWithTime.tss('a);

  open MeasurementValue;
  type marketCdfCdf = {
    agentPredictions: measurementsWithTime(Cdf.t),
    marketPredictions: measurementsWithTime(Cdf.t),
    resolution: measurementWithTime(Cdf.t),
    sampleCount: int,
  };

  type marketCdfFloat = {
    agentPredictions: measurementsWithTime(Cdf.t),
    marketPredictions: measurementsWithTime(Cdf.t),
    resolution: measurementWithTime(float),
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

  let toResolution =
      (time, measurementValue: 'a)
      : TypedMeasurementWithTime.TypedMeasurementWithTime.t('a) => {
    time,
    measurementValue,
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

    let resolutionTime = resolution.time;

    switch (
      marketScoreType,
      typedAgentPredictions,
      typedMarketPredictions,
      resolution,
      sampleCount,
    ) {
    | (
        MarketScore,
        Some(`Cdf(agentPredictions)),
        Some(`Cdf(marketPredictions)),
        {measurementValue: `Float(result)},
        _,
      ) =>
      Some(
        `MarketCdfFloat(
          {
            agentPredictions,
            marketPredictions,
            resolution: toResolution(resolutionTime, result),
          }: ValidScoringCombinationGroupOverTime.marketCdfFloat,
        ),
      )
    | (
        MarketScore,
        Some(`Cdf(agentPredictions)),
        Some(`Cdf(marketPredictions)),
        {measurementValue: `Cdf(result)},
        Some(sampleCount),
      ) =>
      Some(
        `MarketCdfCdf(
          {
            agentPredictions,
            marketPredictions,
            resolution: toResolution(resolutionTime, result),
            sampleCount,
          }: ValidScoringCombinationGroupOverTime.marketCdfCdf,
        ),
      )
    | (
        MarketScore,
        Some(`Percentage(agentPredictions)),
        Some(`Percentage(marketPredictions)),
        {measurementValue: `Percentage(result)},
        _,
      ) =>
      Some(
        `MarketPercentagePercentage(
          {
            agentPredictions,
            marketPredictions,
            resolution: toResolution(resolutionTime, result),
          }: ValidScoringCombinationGroupOverTime.marketPercentagePercentage,
        ),
      )
    | (
        NonMarketScore,
        Some(`Cdf(agentPredictions)),
        _,
        {measurementValue: `Cdf(result)},
        Some(sampleCount),
      ) =>
      Some(
        `NonMarketCdfCdf(
          {
            agentPredictions,
            resolution: toResolution(resolutionTime, result),
            sampleCount,
          }: ValidScoringCombinationGroupOverTime.nonMarketCdfCdf,
        ),
      )
    | (
        NonMarketScore,
        Some(`Cdf(agentPredictions)),
        _,
        {measurementValue: `Float(result)},
        _,
      ) =>
      Some(
        `NonMarketCdfFloat(
          {
            agentPredictions,
            resolution: toResolution(resolutionTime, result),
          }: ValidScoringCombinationGroupOverTime.nonMarketCdfFloat,
        ),
      )
    | (
        NonMarketScore,
        Some(`Percentage(agentPredictions)),
        _,
        {measurementValue: `Percentage(result)},
        _,
      ) =>
      Some(
        `NonMarketPercentagePercentage(
          {
            agentPredictions,
            resolution: toResolution(resolutionTime, result),
          }: ValidScoringCombinationGroupOverTime.nonMarketPercentagePercentage,
        ),
      )
    | _ => None
    };
  };
};