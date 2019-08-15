const { API } = require('./api');

describe('GitHub API', () => {
  it('class should be a constructor', () => {
    expect(API).toBeInstanceOf(Function);
  });
});
