const index = require('./Data');

describe('Data', () => {
  it('returns as an object', () => {
    expect(index.Sampler).toBeInstanceOf(Object);
  });
});
