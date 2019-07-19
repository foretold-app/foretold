const index = require('./auth0');

describe('Auth0 Lib', () => {
  it('returns an object', () => {
    expect(index).toBeInstanceOf(Object);
  });
});
