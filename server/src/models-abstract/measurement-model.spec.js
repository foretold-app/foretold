const { MeasurementModel } = require('./measurement-model');

describe('Measurement Model', () => {
  it('class should be a constructor', () => {
    expect(MeasurementModel).toBeInstanceOf(Function);
  });
});
