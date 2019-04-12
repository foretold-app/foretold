const { MeasurableModel } = require('./measurable-model');

describe('Measurable Model', () => {
  it('class should be a constructor', () => {
    expect(MeasurableModel).toBeInstanceOf(Function);
  });
});
