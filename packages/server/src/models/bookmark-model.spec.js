const { BookmarkModel } = require('./bookmark-model');

describe('Bookmark Model', () => {
  it('class should be a constructor', () => {
    expect(BookmarkModel).toBeInstanceOf(Function);
  });
});
