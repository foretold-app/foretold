type fooType =
  TypedMeasurementWithTime.TypedMeasurementWithTime.tss(
    MeasurementValue.Cdf.t,
  );

module ValidScoringCombinationGroupOverTime = {
  type time = int;

  // open TypedMeasurementWithTime;
  type measurementWithTime('a) =
    TypedMeasurementWithTime.TypedMeasurementWithTime.t('a);

  type measurementsWithTime('a) =
    TypedMeasurementWithTime.TypedMeasurementWithTime.tss('a);

  open MeasurementValue;
  type marketCdfCdf = {
    agentPredictions:
      TypedMeasurementWithTime.TypedMeasurementWithTime.tss(Cdf.t),
    marketPredictions:
      TypedMeasurementWithTime.TypedMeasurementWithTime.tss(Cdf.t),
    resolution: TypedMeasurementWithTime.TypedMeasurementWithTime.t(Cdf.t),
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
      sampleCount,
    ) {
    | (
        MarketScore,
        Some(`Cdf(agentPredictions)),
        Some(`Cdf(marketPredictions)),
        {measurement: `Float(result), time},
        _,
      ) =>
      Some(
        `MarketCdfFloat(
          {
            agentPredictions,
            marketPredictions,
            resolution: {
              time,
              measurement: result,
            },
          }: ValidScoringCombinationGroupOverTime.marketCdfFloat,
        ),
      )
    | (
        MarketScore,
        Some(`Cdf(agentPredictions)),
        Some(`Cdf(marketPredictions)),
        {measurement: `Cdf(result), time},
        Some(sampleCount),
      ) =>
      Some(
        `MarketCdfCdf(
          {
            agentPredictions,
            marketPredictions,
            resolution: {
              time,
              measurement: result,
            },
            sampleCount,
          }: ValidScoringCombinationGroupOverTime.marketCdfCdf,
        ),
      )
    | (
        MarketScore,
        Some(`Percentage(agentPredictions)),
        Some(`Percentage(marketPredictions)),
        {measurement: `Percentage(result), time},
        _,
      ) =>
      Some(
        `MarketPercentagePercentage(
          {
            agentPredictions,
            marketPredictions,
            resolution: {
              time,
              measurement: result,
            },
          }: ValidScoringCombinationGroupOverTime.marketPercentagePercentage,
        ),
      )
    | (
        NonMarketScore,
        Some(`Cdf(agentPredictions)),
        _,
        {measurement: `Cdf(result), time},
        Some(sampleCount),
      ) =>
      Some(
        `NonMarketCdfCdf(
          {
            agentPredictions,
            resolution: {
              time,
              measurement: result,
            },
            sampleCount,
          }: ValidScoringCombinationGroupOverTime.nonMarketCdfCdf,
        ),
      )
    | (
        NonMarketScore,
        Some(`Cdf(agentPredictions)),
        _,
        {measurement: `Float(result), time},
        _,
      ) =>
      Some(
        `NonMarketCdfFloat(
          {
            agentPredictions,
            resolution: {
              time,
              measurement: result,
            },
          }: ValidScoringCombinationGroupOverTime.nonMarketCdfFloat,
        ),
      )
    | (
        NonMarketScore,
        Some(`Percentage(agentPredictions)),
        _,
        {measurement: `Percentage(result), time},
        _,
      ) =>
      Some(
        `NonMarketPercentagePercentage(
          {
            agentPredictions,
            resolution: {
              time,
              measurement: result,
            },
          }: ValidScoringCombinationGroupOverTime.nonMarketPercentagePercentage,
        ),
      )
    | _ => None
    };
  };
};