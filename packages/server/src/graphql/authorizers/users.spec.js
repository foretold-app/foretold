const users = require('./users');

describe('Users Authorizers', () => {
  it('returns as an object', () => {
    expect(users).toBeInstanceOf(Object);
  });
});
