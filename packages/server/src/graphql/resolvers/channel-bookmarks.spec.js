const bookmarks = require('./channel-bookmarks');

describe('Channel Bookmarks Resolvers', () => {
  it('class should be object', () => {
    expect(bookmarks).toBeInstanceOf(Object);
  });
});
