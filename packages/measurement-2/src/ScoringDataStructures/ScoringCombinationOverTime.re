// module MarketCombination
module MeasurementGroupOverTime = {
  type measurementWithTime('a) =
    TypedMeasurementWithTime.TypedMeasurementWithTime.t('a);

  type measurementsWithTime('a) =
    TypedMeasurementWithTime.TypedMeasurementWithTime.tss('a);

  open MeasurementValue;

  type marketCombination('a, 'b) = {
    agentPredictions: measurementsWithTime('a),
    marketPredictions: measurementsWithTime('a),
    resolution: measurementWithTime('b),
  };

  type nonMarketCombination('a, 'b) = {
    agentPredictions: measurementsWithTime('a),
    resolution: measurementWithTime('b),
  };

  type marketCdfCdf = marketCombination(Cdf.t, Cdf.t);
  type marketCdfFloat = marketCombination(Cdf.t, float);
  type marketPercentagePercentage =
    marketCombination(Percentage.t, Percentage.t);

  type marketToMeasurement('a, 'b) =
    ScoringCombination.ValidScoringCombination.marketCombination('a, 'b) =>
    ScoringCombination.ValidScoringCombination.measurements;

  type nonMarketToMeasurement('a, 'b) =
    ScoringCombination.ValidScoringCombination.nonMarketCombination('a, 'b) =>
    ScoringCombination.ValidScoringCombination.measurements;

  let toMarketStartAtDistribution =
      (
        r: marketCombination('a, 'b),
        toMeasurement: marketToMeasurement('a, 'b),
      ) => {
    let finalTime = r.resolution.time;
    let product =
      StartAtDistribution.product(
        r.agentPredictions
        |> TypedMeasurementWithTime.TypedMeasurementWithTime.toStartAtDistribution(
             finalTime,
           ),
        r.marketPredictions
        |> TypedMeasurementWithTime.TypedMeasurementWithTime.toStartAtDistribution(
             finalTime,
           ),
      );
    let toScoringCombinations =
      StartAtDistribution.map(
        ((agentPrediction, marketPrediction)) =>
          ScoringCombination.ValidScoringCombination.make(
            ~measurements=
              toMeasurement({
                agentPrediction,
                marketPrediction,
                resolution: r.resolution.measurementValue,
              }),
            (),
          ),
        product,
      );
    4.5;
  };

  let toNonmarketStartAtDistribution =
      (
        r: nonMarketCombination('a, 'b),
        toMeasurement: nonMarketToMeasurement('a, 'b),
      ) => {
    let finalTime = r.resolution.time;
    let product =
      r.agentPredictions
      |> TypedMeasurementWithTime.TypedMeasurementWithTime.toStartAtDistribution(
           finalTime,
         );
    let toScoringCombinations =
      StartAtDistribution.map(
        agentPrediction =>
          ScoringCombination.ValidScoringCombination.make(
            ~measurements=
              toMeasurement({
                agentPrediction,
                resolution: r.resolution.measurementValue,
              }),
            (),
          ),
        product,
      );
    4.5;
  };

  type nonMarketCdfCdf = nonMarketCombination(Cdf.t, Cdf.t);
  type nonMarketCdfFloat = nonMarketCombination(Cdf.t, float);
  type nonMarketPercentagePercentage =
    nonMarketCombination(Percentage.t, Percentage.t);

  type t = [
    | `MarketCdfCdf(marketCdfCdf)
    | `MarketCdfFloat(marketCdfFloat)
    | `MarketPercentagePercentage(marketPercentagePercentage)
    | `NonMarketCdfCdf(nonMarketCdfCdf)
    | `NonMarketCdfFloat(nonMarketCdfFloat)
    | `NonMarketPercentagePercentage(nonMarketPercentagePercentage)
  ];

  let toStartAtDistribution = (r: t) => {
    switch (r) {
    | `MarketCdfCdf(v) =>
      toMarketStartAtDistribution(v, b => `MarketCdfCdf(b))
    | `MarketCdfFloat(v) =>
      toMarketStartAtDistribution(v, b => `MarketCdfFloat(b))
    | `MarketPercentagePercentage(v) =>
      toMarketStartAtDistribution(v, b => `MarketPercentagePercentage(b))
    | `NonMarketCdfCdf(v) =>
      toNonmarketStartAtDistribution(v, b => `NonMarketCdfCdf(b))
    | `NonMarketCdfFloat(v) =>
      toNonmarketStartAtDistribution(v, b => `NonMarketCdfFloat(b))
    | `NonMarketPercentagePercentage(v) =>
      toNonmarketStartAtDistribution(v, b =>
        `NonMarketPercentagePercentage(b)
      )
    };
  };
};

module ValidScoringCombinationGroupOverTime = {
  type time = int;

  type t = {
    measurementGroup: MeasurementGroupOverTime.t,
    beginningTime: time,
    sampleCount: int,
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
    ) {
    | (
        MarketScore,
        Some(`Cdf(agentPredictions)),
        Some(`Cdf(marketPredictions)),
        {measurementValue: `Float(result)},
      ) =>
      Some(
        `MarketCdfFloat(
          {
            agentPredictions,
            marketPredictions,
            resolution: toResolution(resolutionTime, result),
          }: MeasurementGroupOverTime.marketCdfFloat,
        ),
      )
    | (
        MarketScore,
        Some(`Cdf(agentPredictions)),
        Some(`Cdf(marketPredictions)),
        {measurementValue: `Cdf(result)},
      ) =>
      Some(
        `MarketCdfCdf(
          {
            agentPredictions,
            marketPredictions,
            resolution: toResolution(resolutionTime, result),
          }: MeasurementGroupOverTime.marketCdfCdf,
        ),
      )
    | (
        MarketScore,
        Some(`Percentage(agentPredictions)),
        Some(`Percentage(marketPredictions)),
        {measurementValue: `Percentage(result)},
      ) =>
      Some(
        `MarketPercentagePercentage(
          {
            agentPredictions,
            marketPredictions,
            resolution: toResolution(resolutionTime, result),
          }: MeasurementGroupOverTime.marketPercentagePercentage,
        ),
      )
    | (
        NonMarketScore,
        Some(`Cdf(agentPredictions)),
        _,
        {measurementValue: `Cdf(result)},
      ) =>
      Some(
        `NonMarketCdfCdf(
          {
            agentPredictions,
            resolution: toResolution(resolutionTime, result),
          }: MeasurementGroupOverTime.nonMarketCdfCdf,
        ),
      )
    | (
        NonMarketScore,
        Some(`Cdf(agentPredictions)),
        _,
        {measurementValue: `Float(result)},
      ) =>
      Some(
        `NonMarketCdfFloat(
          {
            agentPredictions,
            resolution: toResolution(resolutionTime, result),
          }: MeasurementGroupOverTime.nonMarketCdfFloat,
        ),
      )
    | (
        NonMarketScore,
        Some(`Percentage(agentPredictions)),
        _,
        {measurementValue: `Percentage(result)},
      ) =>
      Some(
        `NonMarketPercentagePercentage(
          {
            agentPredictions,
            resolution: toResolution(resolutionTime, result),
          }: MeasurementGroupOverTime.nonMarketPercentagePercentage,
        ),
      )
    | _ => None
    };
  };
};