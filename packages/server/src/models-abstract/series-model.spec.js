const { SeriesModel } = require('./series-model');

describe('Series Model', () => {
  it('class should be a constructor', () => {
    expect(SeriesModel).toBeInstanceOf(Function);
  });
});
