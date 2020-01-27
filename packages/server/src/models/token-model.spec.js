const { TokenModel } = require('./token-model');

describe('Token Model', () => {
  it('class should be a constructor', () => {
    expect(TokenModel).toBeInstanceOf(Function);
  });
});
