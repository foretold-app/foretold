const { NewMeasurement } = require('./new-measurement');

describe('New Measurement Feed Item Producer', () => {
  it('returns an constructor', () => {
    expect(NewMeasurement).toBeInstanceOf(Function);
  });
});
