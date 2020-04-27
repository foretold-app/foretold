const { NewSeries } = require('./new-series');

describe('New Series Feed Item Producer', () => {
  it('returns an constructor', () => {
    expect(NewSeries).toBeInstanceOf(Function);
  });
});
