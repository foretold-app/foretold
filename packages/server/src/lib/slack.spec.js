const index = require('./slack');

describe('Slack Lib', () => {
  it('returns an object', () => {
    expect(index).toBeInstanceOf(Object);
  });
});
