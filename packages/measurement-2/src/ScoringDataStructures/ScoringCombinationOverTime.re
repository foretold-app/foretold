// module MarketCombination
module MeasurementCombionationOverTime = {
  type measurementWithTime('a) =
    TypedMeasurementWithTime.TypedMeasurementWithTime.t('a);

  type measurementsWithTime('a) =
    TypedMeasurementWithTime.TypedMeasurementWithTime.tss('a);

  open MeasurementValue;

  type combination('a, 'b) = {
    agentPredictions: measurementsWithTime('a),
    marketPredictions: option(measurementsWithTime('a)),
    resolution: measurementWithTime('b),
  };

  type cdfCdf = combination(Cdf.t, Cdf.t);
  type cdfFloat = combination(Cdf.t, float);
  type percentagePercentage = combination(Percentage.t, Percentage.t);

  type combinationToMeasurement('a, 'b) =
    ScoringCombination.ScoringCombinationMeasurements.combination('a, 'b) =>
    ScoringCombination.ScoringCombinationMeasurements.t;

  let toStartAtDistribution =
      (
        r: combination('a, 'b),
        toMeasurement: combinationToMeasurement('a, 'b),
      ) => {
    let finalTime = r.resolution.time;
    switch (r.marketPredictions) {
    | Some(marketPredictions) =>
      let product =
        StartAtDistribution.product(
          r.agentPredictions
          |> TypedMeasurementWithTime.TypedMeasurementWithTime.toStartAtDistribution(
               finalTime,
             ),
          marketPredictions
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
                  marketPrediction: Some(marketPrediction),
                  resolution: r.resolution.measurementValue,
                }),
              (),
            ),
          product,
        );
      toScoringCombinations;
    | None =>
      r.agentPredictions
      |> TypedMeasurementWithTime.TypedMeasurementWithTime.toStartAtDistribution(
           finalTime,
         )
      |> StartAtDistribution.map(agentPrediction =>
           ScoringCombination.ValidScoringCombination.make(
             ~measurements=
               toMeasurement({
                 agentPrediction,
                 marketPrediction: None,
                 resolution: r.resolution.measurementValue,
               }),
             (),
           )
         )
    };
  };

  type t = [
    | `CdfCdf(cdfCdf)
    | `CdfFloat(cdfFloat)
    | `PercentagePercentage(percentagePercentage)
  ];
};

module ValidScoringCombinationGroupOverTime = {
  type time = int;

  type t = {
    measurementGroup: MeasurementCombionationOverTime.t,
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
        `CdfFloat(
          {
            agentPredictions,
            marketPredictions: Some(marketPredictions),
            resolution: toResolution(resolutionTime, result),
          }: MeasurementCombionationOverTime.cdfFloat,
        ),
      )
    | (
        MarketScore,
        Some(`Cdf(agentPredictions)),
        Some(`Cdf(marketPredictions)),
        {measurementValue: `Cdf(result)},
      ) =>
      Some(
        `CdfCdf(
          {
            agentPredictions,
            marketPredictions: Some(marketPredictions),
            resolution: toResolution(resolutionTime, result),
          }: MeasurementCombionationOverTime.cdfCdf,
        ),
      )
    | (
        MarketScore,
        Some(`Percentage(agentPredictions)),
        Some(`Percentage(marketPredictions)),
        {measurementValue: `Percentage(result)},
      ) =>
      Some(
        `PercentagePercentage(
          {
            agentPredictions,
            marketPredictions: Some(marketPredictions),
            resolution: toResolution(resolutionTime, result),
          }: MeasurementCombionationOverTime.percentagePercentage,
        ),
      )
    | (
        NonMarketScore,
        Some(`Cdf(agentPredictions)),
        _,
        {measurementValue: `Cdf(result)},
      ) =>
      Some(
        `CdfCdf(
          {
            agentPredictions,
            marketPredictions: None,
            resolution: toResolution(resolutionTime, result),
          }: MeasurementCombionationOverTime.cdfCdf,
        ),
      )
    | (
        NonMarketScore,
        Some(`Cdf(agentPredictions)),
        _,
        {measurementValue: `Float(result)},
      ) =>
      Some(
        `CdfFloat(
          {
            agentPredictions,
            marketPredictions: None,
            resolution: toResolution(resolutionTime, result),
          }: MeasurementCombionationOverTime.cdfFloat,
        ),
      )
    | (
        NonMarketScore,
        Some(`Percentage(agentPredictions)),
        _,
        {measurementValue: `Percentage(result)},
      ) =>
      Some(
        `PercentagePercentage(
          {
            agentPredictions,
            marketPredictions: None,
            resolution: toResolution(resolutionTime, result),
          }: MeasurementCombionationOverTime.percentagePercentage,
        ),
      )
    | _ => None
    };
  };
};