const channelBookmarks = require('./channel-bookmarks');

describe('Channel Bookmarks Authorizers', () => {
  it('returns bots as an object', () => {
    expect(channelBookmarks).toBeInstanceOf(Object);
  });
});
