const { GitHubApi } = require('./git-hub-api');

describe('GitHub API', () => {
  it('class should be a constructor', () => {
    expect(GitHubApi).toBeInstanceOf(Function);
  });
});
