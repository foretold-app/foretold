const permissions = require('./permissions');

describe('Permissions Resolvers', () => {
  it('returns as an object', () => {
    expect(permissions).toBeInstanceOf(Object);
  });
});
