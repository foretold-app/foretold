const { ProducerFeedItems } = require('./producer-feed-items');

describe('Producer Feed Items', () => {
  it('returns an constructor', () => {
    expect(ProducerFeedItems).toBeInstanceOf(Function);
  });
});
