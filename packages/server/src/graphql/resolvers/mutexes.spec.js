const index = require('./mutexes');

describe('Mutexes Resolvers', () => {
  it('returns as an object', () => {
    expect(index).toBeInstanceOf(Object);
  });
});
