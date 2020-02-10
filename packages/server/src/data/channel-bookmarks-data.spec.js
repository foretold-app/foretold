const data = require('./bookmarks-data');

describe('Bookmarks Data', () => {
  it('class should be a constructor', () => {
    expect(data.BotsData).toBeInstanceOf(Function);
  });
});
