const { MeasurablesData } = require('./measurables-data');

describe('tests Measurables Data layer', () => {

  it('class should be a constructor', () => {
    expect(MeasurablesData).toBeInstanceOf(Function);
  });

});
