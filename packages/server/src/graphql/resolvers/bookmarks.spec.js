const bookmarks = require('./bookmarks');

describe('Bookmarks Resolvers', () => {
  it('class should be object', () => {
    expect(bookmarks).toBeInstanceOf(Object);
  });
});
