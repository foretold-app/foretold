const index = require('./DistributionUniform');

describe('DistributionUniform', () => {
  it('returns as an object', () => {
    expect(index.Sampler).toBeInstanceOf(Object);
  });
});
