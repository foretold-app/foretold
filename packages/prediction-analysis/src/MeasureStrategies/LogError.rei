[@genType]
type predictionGroupErrorType = [
  | `MarketScore(PredictionResolutionGroup.WithMarket.t)
  | `NonMarketScore(PredictionResolutionGroup.t)
];

[@genType]
let predictionGroupError:
  (~scoringCombination: predictionGroupErrorType, ~sampleCount: int, unit) =>
  Belt.Result.t(float, 'a);

[@genType]
let differentialEntropy:
  (int, PredictionAnalysis.MeasurementValue.t) => option(float);