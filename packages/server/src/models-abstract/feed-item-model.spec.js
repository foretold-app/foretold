const { FeedItemModel } = require('./feed-item-model');

describe('FeedItem Model', () => {
  it('class should be a constructor', () => {
    expect(FeedItemModel).toBeInstanceOf(Function);
  });
});
