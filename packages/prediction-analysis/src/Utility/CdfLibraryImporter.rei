module Distribution: {
  let toPdf: Types.Distribution.t => Types.Distribution.t;
  let findX: (float, Types.Distribution.t) => float;
  let findY: (float, Types.Distribution.t) => float;
  let integral: Types.Distribution.t => float;
  let differentialEntropy: (int, Types.Distribution.t) => float;
};

module PredictionResolutionGroup: {
  let logScoreNonMarketCdfCdf:
    (
      ~sampleCount: int,
      ~agentPrediction: Types.Distribution.t,
      ~resolution: Types.Distribution.t
    ) =>
    Types.Distribution.t;
};