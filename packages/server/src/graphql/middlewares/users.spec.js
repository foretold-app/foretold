const index = require('./users');

describe('Users Middleware', () => {
  it('returns an object', () => {
    expect(index).toBeInstanceOf(Object);
  });
});
