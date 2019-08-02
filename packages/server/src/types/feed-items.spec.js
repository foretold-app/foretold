const feedItems = require('./feed-items');

describe('Feed Items Types', () => {
  it('common is an object', () => {
    expect(feedItems).toBeInstanceOf(Object);
  });
});
