const { FeedItemNotebook } = require('./feed-item-notebook');

describe('Feed Item Notebook', () => {
  it('returns a constructor', () => {
    expect(FeedItemNotebook).toBeInstanceOf(Function);
  });
});
