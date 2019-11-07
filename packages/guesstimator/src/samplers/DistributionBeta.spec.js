const index = require('./DistributionBeta');

describe('DistributionBeta', () => {
  it('returns as an object', () => {
    expect(index.Sampler).toBeInstanceOf(Object);
  });
});
