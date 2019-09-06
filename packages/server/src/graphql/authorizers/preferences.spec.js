const preferences = require('./preferences');

describe('Preferences Authorizers', () => {
  it('returns as an object', () => {
    expect(preferences).toBeInstanceOf(Object);
  });
});
