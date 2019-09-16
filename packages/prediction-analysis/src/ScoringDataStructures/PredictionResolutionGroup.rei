module WithMarket: {
  type combination('a, 'b) = {
    agentPrediction: 'a,
    marketPrediction: 'a,
    resolution: 'b,
  };

  type t = [
    | `CdfCdf(combination(Cdf.t, Cdf.t))
    | `CdfFloat(combination(Cdf.t, float))
    | `PercentagePercentage(combination(Percentage.t, Percentage.t))
  ];
};

type combination('a, 'b) = {
  agentPrediction: 'a,
  marketPrediction: option('a),
  resolution: 'b,
};

type t = [
  | `CdfCdf(combination(Cdf.t, Cdf.t))
  | `CdfFloat(combination(Cdf.t, float))
  | `PercentagePercentage(combination(Percentage.t, Percentage.t))
];

let fromArbitraryMeasurementValues:
  (
    ~agentPrediction: MeasurementValue.t,
    ~marketPrediction: option(MeasurementValue.t)=?,
    ~resolution: MeasurementValue.t,
    unit
  ) =>
  Belt.Result.t(t, string);

let toWithMarket: t => option(WithMarket.t);