const index = require('./log');

describe('Log Configuration', () => {
  it('returns an object', () => {
    expect(index)
      .toBeInstanceOf(Object);
  });
});
