const measurements = require('./measurements');

describe('Measurements Authorizers', () => {
  it('returns as an object', () => {
    expect(measurements).toBeInstanceOf(Object);
  });
});
