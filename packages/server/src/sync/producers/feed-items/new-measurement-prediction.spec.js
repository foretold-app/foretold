const { NewMeasurementPrediction } = require('./new-measurement-prediction');

describe('New Measurement Prediction Feed Item Producer', () => {
  it('returns an constructor', () => {
    expect(NewMeasurementPrediction).toBeInstanceOf(Function);
  });
});
