const index = require('./channel-bookmarks');

describe('Channel Bookmarks Middleware', () => {
  it('returns an object', () => {
    expect(index).toBeInstanceOf(Object);
  });
});
