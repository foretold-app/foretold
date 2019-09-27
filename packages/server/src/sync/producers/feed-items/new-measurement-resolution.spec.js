const { NewMeasurementResolution } = require('./new-measurement-resolution');

describe('New Measurement Resolution Feed Item Producer', () => {
  it('returns an constructor', () => {
    expect(NewMeasurementResolution).toBeInstanceOf(Function);
  });
});
