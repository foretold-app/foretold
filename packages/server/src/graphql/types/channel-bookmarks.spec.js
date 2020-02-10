const bookmarks = require('./channel-bookmarks');

describe('bookmarks types', () => {
  it('class should be object', () => {
    expect(bookmarks).toBeInstanceOf(Object);
  });
});
