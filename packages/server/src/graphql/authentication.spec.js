const authentication = require('./authentication');

describe('authentication', () => {
  it('class should be object', () => {
    expect(authentication).toBeInstanceOf(Object);
  });
  it('authentication', () => {
    expect(authentication.authentication).toBeInstanceOf(Function);
  });
});
