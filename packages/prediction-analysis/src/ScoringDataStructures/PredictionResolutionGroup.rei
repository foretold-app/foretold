// module WithMarket: {
//   type combination('a, 'b) = {
//     agentPrediction: 'a,
//     marketPrediction: 'a,
//     resolution: 'b,
//   };

//   type cdfCdf = combination(Cdf.t, Cdf.t);
//   type cdfFloat = combination(Cdf.t, float);
//   type percentagePercentage = combination(Percentage.t, Percentage.t);

//   type t = [
//     | `CdfCdf(combination(Cdf.t, Cdf.t))
//     | `CdfFloat(cdfFloat)
//     | `PercentagePercentage(percentagePercentage)
//   ];
// };

type combination('a, 'b) = {
  agentPrediction: 'a,
  marketPrediction: option('a),
  resolution: 'b,
};

type cdfCdf = combination(Cdf.t, Cdf.t);
type cdfFloat = combination(Cdf.t, float);
type percentagePercentage = combination(Percentage.t, Percentage.t);

type t = [
  | `CdfCdf(combination(Cdf.t, Cdf.t))
  | `CdfFloat(cdfFloat)
  | `PercentagePercentage(percentagePercentage)
];

// let toWithMarket: t => Belt.Result.t(WithMarket.t, string);

let fromArbitraryMeasurementValues:
  (
    ~agentPrediction: MeasurementValue.t,
    ~marketPrediction: option(MeasurementValue.t)=?,
    ~resolution: MeasurementValue.t,
    unit
  ) =>
  Belt.Result.t(t, string);

let marketPredictionExists: t => bool;