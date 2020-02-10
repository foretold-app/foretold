const graphql = require('graphql');

const bookmarks = require('./bookmarks');

describe('bookmarks types', () => {
  it('class should be object', () => {
    expect(bookmarks).toBeInstanceOf(Object);
  });
});
