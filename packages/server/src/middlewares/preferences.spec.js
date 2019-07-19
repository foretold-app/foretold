const index = require('./preferences');

describe('Preferences Middleware', () => {
  it('returns an object', () => {
    expect(index).toBeInstanceOf(Object);
  });
});
