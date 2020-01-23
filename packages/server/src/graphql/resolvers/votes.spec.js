const index = require('./votes');

describe('Votes Resolvers', () => {
  it('returns as an object', () => {
    expect(index).toBeInstanceOf(Object);
  });
});
