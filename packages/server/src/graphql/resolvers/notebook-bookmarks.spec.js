const bookmarks = require('./notebook-bookmarks');

describe('Notebook Bookmarks Resolvers', () => {
  it('class should be object', () => {
    expect(bookmarks).toBeInstanceOf(Object);
  });
});
