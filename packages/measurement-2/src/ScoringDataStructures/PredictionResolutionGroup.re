type combination('a, 'b) = {
  agentPrediction: 'a,
  marketPrediction: option('a),
  resolution: 'b,
};
type cdfCdf = combination(Cdf.t, Cdf.t);
type cdfFloat = combination(Cdf.t, float);
type percentagePercentage = combination(Percentage.t, Percentage.t);
type sum = [
  | `CdfCdf(cdfCdf)
  | `CdfFloat(cdfFloat)
  | `PercentagePercentage(percentagePercentage)
];
type t = sum;

module ArbitraryMeasurementValueGroup = {
  type t = {
    agentPrediction: MeasurementValue.t,
    marketPrediction: option(MeasurementValue.t),
    resolution: MeasurementValue.t,
  };

  let make = (~agentPrediction, ~marketPrediction=None, ~resolution, ()) => {
    agentPrediction,
    marketPrediction,
    resolution,
  };

  let toValidPredictionResolutionGroup =
      ({agentPrediction, resolution, marketPrediction}: t)
      : Belt.Result.t(sum, string) => {
    let measurements: Belt.Result.t(sum, string) =
      switch (agentPrediction, marketPrediction, resolution) {
      | (
          `Cdf(agentPrediction),
          Some(`Cdf(marketPrediction)),
          `Cdf(resolution),
        ) =>
        Ok(
          `CdfCdf({
            agentPrediction,
            marketPrediction: Some(marketPrediction),
            resolution,
          }),
        )
      | (
          `Cdf(agentPrediction),
          Some(`Cdf(marketPrediction)),
          `Float(resolution),
        ) =>
        Ok(
          `CdfFloat({
            agentPrediction,
            marketPrediction: Some(marketPrediction),
            resolution,
          }),
        )
      | (
          `Percentage(agentPrediction),
          Some(`Percentage(marketPrediction)),
          `Percentage(resolution),
        ) =>
        Ok(
          `PercentagePercentage({
            agentPrediction,
            marketPrediction: Some(marketPrediction),
            resolution,
          }),
        )
      | (`Cdf(agentPrediction), None, `Cdf(resolution)) =>
        Ok(`CdfCdf({agentPrediction, marketPrediction: None, resolution}))
      | (`Cdf(agentPrediction), None, `Float(resolution)) =>
        Ok(`CdfFloat({agentPrediction, marketPrediction: None, resolution}))
      | (`Percentage(agentPrediction), None, `Percentage(resolution)) =>
        Ok(
          `PercentagePercentage({
            agentPrediction,
            marketPrediction: None,
            resolution,
          }),
        )
      | _ =>
        Error(
          "#toValidScoringCombination error: Scoring Combination Not Valid.",
        )
      };
    measurements;
  };
};

module T = {
  let fromArbitraryMeasurementValues =
      (~agentPrediction, ~marketPrediction=None, ~resolution, ()) =>
    ArbitraryMeasurementValueGroup.make(
      ~agentPrediction,
      ~marketPrediction,
      ~resolution,
      (),
    )
    |> ArbitraryMeasurementValueGroup.toValidPredictionResolutionGroup;

  let marketPredictionIsSome = (m: t) =>
    switch (m) {
    | `CdfCdf({marketPrediction: Some(_)}) => true
    | `CdfFloat({marketPrediction: Some(_)}) => true
    | `PercentagePercentage({marketPrediction: Some(_)}) => true
    | `CdfCdf({marketPrediction: None}) => false
    | `CdfFloat({marketPrediction: None}) => false
    | `PercentagePercentage({marketPrediction: None}) => false
    };
};

include T;