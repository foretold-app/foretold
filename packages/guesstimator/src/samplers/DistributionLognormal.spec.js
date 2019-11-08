const index = require('./DistributionLognormal');

describe('DistributionLognormal', () => {
  it('returns as an object', () => {
    expect(index.Sampler).toBeInstanceOf(Object);
  });
});
