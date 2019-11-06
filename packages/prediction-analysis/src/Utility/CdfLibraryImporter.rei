module Distribution: {
  let toPdf: Types.distribution => Types.distribution;
  let findX: (float, PredictionAnalysis.Types.distribution) => float;
  let findY: (float, PredictionAnalysis.Types.distribution) => float;
  let integral: Types.distribution => float;
  let differentialEntropy:
    (int, PredictionAnalysis.Types.distribution) => float;
};

module PredictionResolutionGroup: {
  let logScoreNonMarketCdfCdf:
    (
      ~sampleCount: int,
      ~agentPrediction: PredictionAnalysis.Types.distribution,
      ~resolution: PredictionAnalysis.Types.distribution
    ) =>
    float;
};