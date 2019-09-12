type combination('a, 'b) = {
  agentPrediction: 'a,
  marketPrediction: option('a),
  resolution: 'b,
};

type cdfCdf = combination(Cdf.t, Cdf.t);
type cdfFloat = combination(Cdf.t, float);
type percentagePercentage = combination(Percentage.t, Percentage.t);

type t = [
  | `CdfCdf(cdfCdf)
  | `CdfFloat(cdfFloat)
  | `PercentagePercentage(percentagePercentage)
];

let fromArbitraryMeasurementValues:
  (
    ~agentPrediction: Measurement.MeasurementValue.t,
    ~marketPrediction: option(Measurement.MeasurementValue.t)=?,
    ~resolution: Measurement.MeasurementValue.t,
    unit
  ) =>
  Belt.Result.t(t, string);

let marketPredictionExists: t => bool;