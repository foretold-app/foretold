const votes = require('./votes');

describe('votes Types', () => {
  it('returns an object', () => {
    expect(votes)
      .toBeInstanceOf(Object);
  });
});
