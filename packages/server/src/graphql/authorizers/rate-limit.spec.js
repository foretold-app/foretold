const rateLimit = require('./rate-limit');

describe('Rate Limit Authorizers', () => {
  it('returns as an object', () => {
    expect(rateLimit).toBeInstanceOf(Object);
  });
});
