const channels = require('./channels');

describe('Channels Authorizers', () => {
  it('returns as an object', () => {
    expect(channels).toBeInstanceOf(Object);
  });
});
