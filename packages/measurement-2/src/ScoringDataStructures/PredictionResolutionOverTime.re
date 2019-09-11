module MeasurementCombinationOverTime = {
  type measurementWithTime = MeasurementWithTime.t;
  type measurementsWithTime = MeasurementWithTime.Ts.Uniform.us;

  type combination = {
    agentPredictions: measurementsWithTime,
    marketPredictions: option(measurementsWithTime),
    resolution: measurementWithTime,
  };

  type t = [
    | `CdfCdf(combination)
    | `CdfFloat(combination)
    | `PercentagePercentage(combination)
  ];

  let _toStartAtDistribution =
      (
        r: combination,
        toMeasurement:
          PredictionResolutionGroup.combination('a, 'b) =>
          PredictionResolutionGroup.t,
      ) => {
    let toDistribution =
      MeasurementWithTime.Ts.toStartAtDistribution(r.resolution.time);
    switch (r.marketPredictions) {
    | Some(marketPredictions) =>
      let product =
        StartAtDistribution.product(
          toDistribution(marketPredictions),
          toDistribution(r.agentPredictions),
        );
      let toScoringCombinations =
        StartAtDistribution.map(
          ((agentPrediction, marketPrediction)) =>
            toMeasurement({
              agentPrediction,
              marketPrediction: Some(marketPrediction),
              resolution: r.resolution.measurementValue,
            }),
          product,
        );
      toScoringCombinations;
    | None =>
      r.agentPredictions
      |> toDistribution
      |> StartAtDistribution.map(agentPrediction =>
           toMeasurement({
             agentPrediction,
             marketPrediction: None,
             resolution: r.resolution.measurementValue,
           })
         )
    };
  };

  let toStartAtDistribution = (t: t) =>
    switch (t) {
    | `CdfCdf(v) => _toStartAtDistribution(v, r => `CdfCdf(r))
    | `CdfFloat(v) => _toStartAtDistribution(v, r => `CdfFloat(r))
    | `PercentagePercentage(v) =>
      _toStartAtDistribution(v, r => `PercentagePercentage(r))
    };
};

// TODO: Delete this, the beginningTIme isn't important
module ValidScoringCombinationGroupOverTime = {
  type time = float;

  type t = {
    measurementGroup: MeasurementCombinationOverTime.t,
    beginningTime: time,
  };

  let make = (t: t) => t;
};

// Check the resolution before checking other params.
module ScoringCombinationGroupOverTimeInput = {
  type t = {
    agentPredictions: TypedMeasurementWithTime.MeasurementWithTime.ts,
    marketPredictions:
      option(TypedMeasurementWithTime.MeasurementWithTime.ts),
    resolution: TypedMeasurementWithTime.MeasurementWithTime.t,
  };

  let _toResolution =
      (time, measurementValue: 'a): TypedMeasurementWithTime.T.t('a) => {
    time,
    measurementValue,
  };

  let toValidScoringCombination =
      ({agentPredictions, marketPredictions, resolution}) => {
    open TypedMeasurementWithTime.MeasurementWithTime;
    let typedAgentPredictions = agentPredictions |> toTypeOfFirstElement;
    let typedMarketPredictions =
      marketPredictions |> Rationale.Option.bind(_, toTypeOfFirstElement);

    let resolutionTime = resolution.time;

    switch (typedAgentPredictions, typedMarketPredictions, resolution) {
    | (
        Some(`Cdf(agentPredictions)),
        Some(`Cdf(marketPredictions)),
        {measurementValue: `Float(result)},
      ) =>
      Some(
        `CdfFloat(
          {
            agentPredictions,
            marketPredictions: Some(marketPredictions),
            resolution: _toResolution(resolutionTime, result),
          }: MeasurementCombinationOverTime.cdfFloat,
        ),
      )
    | (
        Some(`Cdf(agentPredictions)),
        Some(`Cdf(marketPredictions)),
        {measurementValue: `Cdf(result)},
      ) =>
      Some(
        `CdfCdf(
          {
            agentPredictions,
            marketPredictions: Some(marketPredictions),
            resolution: _toResolution(resolutionTime, result),
          }: MeasurementCombinationOverTime.cdfCdf,
        ),
      )
    | (
        Some(`Percentage(agentPredictions)),
        Some(`Percentage(marketPredictions)),
        {measurementValue: `Percentage(result)},
      ) =>
      Some(
        `PercentagePercentage(
          {
            agentPredictions,
            marketPredictions: Some(marketPredictions),
            resolution: _toResolution(resolutionTime, result),
          }: MeasurementCombinationOverTime.percentagePercentage,
        ),
      )
    | (Some(`Cdf(agentPredictions)), _, {measurementValue: `Cdf(result)}) =>
      Some(
        `CdfCdf(
          {
            agentPredictions,
            marketPredictions: None,
            resolution: _toResolution(resolutionTime, result),
          }: MeasurementCombinationOverTime.cdfCdf,
        ),
      )
    | (
        Some(`Cdf(agentPredictions)),
        _,
        {measurementValue: `Float(result)},
      ) =>
      Some(
        `CdfFloat(
          {
            agentPredictions,
            marketPredictions: None,
            resolution: _toResolution(resolutionTime, result),
          }: MeasurementCombinationOverTime.cdfFloat,
        ),
      )
    | (
        Some(`Percentage(agentPredictions)),
        _,
        {measurementValue: `Percentage(result)},
      ) =>
      Some(
        `PercentagePercentage(
          {
            agentPredictions,
            marketPredictions: None,
            resolution: _toResolution(resolutionTime, result),
          }: MeasurementCombinationOverTime.percentagePercentage,
        ),
      )
    | _ => None
    };
  };
};