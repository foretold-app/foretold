type marketScoreMeasurements = {
  prediction: Types.Measurement.t,
  market: Types.Measurement.t,
  resolution: Types.Measurement.t,
};

type nonMarketScoreMeasurements = {
  prediction: Types.Measurement.t,
  resolution: Types.Measurement.t,
};

type t = {
  measurements: [
    | `marketScoreMeasurements(marketScoreMeasurements)
    | `nonMarketScoreMeasurements(nonMarketScoreMeasurements)
  ],
  sampleCount: int,
};