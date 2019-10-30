const index = require('./notebooks');

describe('Notebooks  Resolvers', () => {
  it('returns as an object', () => {
    expect(index).toBeInstanceOf(Object);
  });
});
