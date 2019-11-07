const index = require('./DistributionNormal');

describe('DistributionNormal', () => {
  it('returns as an object', () => {
    expect(index.Sampler).toBeInstanceOf(Object);
  });
});
