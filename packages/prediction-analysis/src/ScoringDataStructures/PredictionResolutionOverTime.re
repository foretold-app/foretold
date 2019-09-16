module MeasurementWithTime = {
  type time = float;

  type _t('a) = {
    time,
    measurementValue: 'a,
  };

  let make = (~time, ~measurementValue) => {measurementValue, time};

  module MeasurementWithTimeInput = {
    type t('a) = _t('a);
    let toMeasurement = (t: t('a)): 'a => t.measurementValue;
    let map = (t: t('a), fn: 'a => 'b) =>
      make(~time=t.time, ~measurementValue=fn(t.measurementValue));
  };

  module Ts = {
    include MeasurementValueCollectionFunctor.Make(MeasurementWithTimeInput);
    let toStartAtDistribution = (finalTime, ts: ls('a)) =>
      StartAtDistribution.make(
        ~finalX=finalTime,
        ~pointXY=
          ts
          |> Array.map(({time, measurementValue}) =>
               (time, measurementValue)
             ),
      );
  };

  include Ts;
};

module T = {
  type measurementWithTime('a) = MeasurementWithTime.l('a);
  type measurementsWithTime('a) = MeasurementWithTime.ls('a);

  type combination('a, 'b) = {
    agentPredictions: measurementsWithTime('a),
    marketPredictions: option(measurementsWithTime('a)),
    resolution: measurementWithTime('b),
  };

  type cdfCdf = combination(Cdf.t, Cdf.t);
  type cdfFloat = combination(Cdf.t, float);
  type percentagePercentage = combination(Percentage.t, Percentage.t);

  type t = [
    | `CdfCdf(cdfCdf)
    | `CdfFloat(cdfFloat)
    | `PercentagePercentage(percentagePercentage)
  ];

  let _toStartAtDistribution =
      (
        r: combination('a, 'b),
        toMeasurement:
          PredictionResolutionGroup.combination('a, 'b) =>
          PredictionResolutionGroup.t,
      ) => {
    let toDistribution = (m: measurementsWithTime('a)) =>
      m |> MeasurementWithTime.toStartAtDistribution(r.resolution.time);
    switch (r.marketPredictions) {
    | Some(marketPredictions) =>
      let product: StartAtDistribution.t(MeasurementWithTime.time, ('a, 'a)) =
        StartAtDistribution.product(
          toDistribution(r.agentPredictions),
          toDistribution(marketPredictions),
        );
      let toScoringCombinations =
        StartAtDistribution.map(
          ((agentPrediction: 'a, marketPrediction: 'a)) =>
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

module TBuilder = {
  type t = {
    agentPredictions: MeasurementWithTime.ts,
    marketPredictions: option(MeasurementWithTime.ts),
    resolution: MeasurementWithTime.t,
  };

  let hasValidPredictionPair = (t: t): bool => {
    let {agentPredictions, resolution} = t;
    let hasMoreThanOne = typeName =>
      MeasurementWithTime.lengthOfIntendedType(typeName, agentPredictions) > 0;
    switch (resolution.measurementValue) {
    | `Float(_) when hasMoreThanOne(`Cdf) => true
    | `Cdf(_) when hasMoreThanOne(`Cdf) => true
    | `Percentage(_) when hasMoreThanOne(`Percentage) => true
    | `Binary(_) when hasMoreThanOne(`Percentage) => true
    | _ => false
    };
  };

  let validateValidPredictionPair = (t: t): Belt.Result.t(t, string) =>
    t |> hasValidPredictionPair ? Ok(t) : Error("Prediction pair not valid");

  let _run =
      ({agentPredictions, marketPredictions, resolution}: t)
      : Belt.Result.t(T.t, string) => {
    let res = resolution;
    open MeasurementWithTime;
    let convertAgent = typeName => {
      Unwrapped.fromT(typeName, agentPredictions);
    };
    let convertMarket = typeName => {
      marketPredictions
      |> Rationale.Option.bind(_, r =>
           r |> Unwrapped.fromT(typeName) |> Rationale.Option.ofResult
         );
    };
    switch (resolution.measurementValue) {
    | `Float(resolution) =>
      switch (convertAgent(`Cdf), convertMarket(`Cdf)) {
      | (Ok(`Cdf(agentPredictions)), Some(`Cdf(marketPredictions))) =>
        Belt.Result.Ok(
          `CdfFloat(
            {
              agentPredictions,
              marketPredictions: Some(marketPredictions),
              resolution: {
                time: res.time,
                measurementValue: resolution,
              },
            }: T.cdfFloat,
          ),
        )
      | (Ok(`Cdf(agentPredictions)), None) =>
        Ok(
          `CdfFloat(
            {
              agentPredictions,
              marketPredictions: None,
              resolution: {
                time: res.time,
                measurementValue: resolution,
              },
            }: T.cdfFloat,
          ),
        )
      | (Error(e), _) => Error(e)
      | (Ok(_), _) => Error("This should be an impossible error.")
      }
    | `Cdf(resolution) =>
      switch (convertAgent(`Cdf), convertMarket(`Cdf)) {
      | (Ok(`Cdf(agentPredictions)), Some(`Cdf(marketPredictions))) =>
        Ok(
          `CdfCdf(
            {
              agentPredictions,
              marketPredictions: Some(marketPredictions),
              resolution: {
                time: res.time,
                measurementValue: resolution,
              },
            }: T.cdfCdf,
          ),
        )
      | (Ok(`Cdf(agentPredictions)), None) =>
        Ok(
          `CdfCdf(
            {
              agentPredictions,
              marketPredictions: None,
              resolution: {
                time: res.time,
                measurementValue: resolution,
              },
            }: T.cdfCdf,
          ),
        )
      | (Error(e), _) => Error(e)
      | (Ok(_), _) => Error("This should be an impossible error.")
      }
    | `Percentage(resolution) =>
      switch (convertAgent(`Percentage), convertMarket(`Percentage)) {
      | (
          Ok(`Percentage(agentPredictions)),
          Some(`Percentage(marketPredictions)),
        ) =>
        Ok(
          `PercentagePercentage(
            {
              agentPredictions,
              marketPredictions: Some(marketPredictions),
              resolution: {
                time: res.time,
                measurementValue: resolution,
              },
            }: T.percentagePercentage,
          ),
        )
      | (Ok(`Percentage(agentPredictions)), None) =>
        Ok(
          `PercentagePercentage(
            {
              agentPredictions,
              marketPredictions: None,
              resolution: {
                time: res.time,
                measurementValue: resolution,
              },
            }: T.percentagePercentage,
          ),
        )
      | (Error(e), _) => Error(e)
      | (Ok(_), _) => Error("This should be an impossible error.")
      }
    | `Binary(resolution) =>
      switch (convertAgent(`Percentage), convertMarket(`Percentage)) {
      | (
          Ok(`Percentage(agentPredictions)),
          Some(`Percentage(marketPredictions)),
        ) =>
        Ok(
          `PercentagePercentage(
            {
              agentPredictions,
              marketPredictions: Some(marketPredictions),
              resolution: {
                time: res.time,
                measurementValue: Percentage.fromBool(resolution),
              },
            }: T.percentagePercentage,
          ),
        )
      | (Ok(`Percentage(agentPredictions)), None) =>
        Ok(
          `PercentagePercentage(
            {
              agentPredictions,
              marketPredictions: None,
              resolution: {
                time: res.time,
                measurementValue: Percentage.fromBool(resolution),
              },
            }: T.percentagePercentage,
          ),
        )
      | (Error(e), _) => Error(e)
      | (Ok(_), _) => Error("This should be an impossible error.")
      }
    | _ => Error("Grouping not allowed")
    };
  };

  let run =
      (
        ~agentPredictions: MeasurementWithTime.ts,
        ~marketPredictions: option(MeasurementWithTime.ts)=None,
        ~resolution: MeasurementWithTime.t,
      ) => {
    let input = {agentPredictions, marketPredictions, resolution};
    input |> validateValidPredictionPair |> Belt.Result.flatMap(_, _run);
  };
};

include T;
let fromMeasurementCombination = TBuilder.run;