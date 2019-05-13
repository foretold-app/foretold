const data = require('./tokens-data');

describe('Tokens Base', () => {
  it('class should be a constructor', () => {
    expect(data.TokensData).toBeInstanceOf(Function);
  });
});
