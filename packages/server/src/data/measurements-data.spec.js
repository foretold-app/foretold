const { MeasurementsData } = require('./measurements-data');

describe('Measurement Data layer', () => {
  it('class should be a constructor', () => {
    expect(MeasurementsData).toBeInstanceOf(Function);
  });
});
