const {
  NewMeasurementNotAvailable,
} = require('./new-measurement-not-available');

describe('New Measurement Not Available Feed Item Producer', () => {
  it('returns an constructor', () => {
    expect(NewMeasurementNotAvailable).toBeInstanceOf(Function);
  });
});
