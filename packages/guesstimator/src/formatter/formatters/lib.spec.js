const index = require('./lib');

describe('Lib', () => {
  it('returns as an object', () => {
    expect(index.regexBasedFormatter).toBeInstanceOf(Function);
  });
});
