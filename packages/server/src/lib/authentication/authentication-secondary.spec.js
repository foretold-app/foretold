const data = require('./authentication-secondary');

describe('AuthenticationSecondary Data', () => {
  it('class should be a constructor', () => {
    expect(data.AuthenticationSecondary).toBeInstanceOf(Function);
  });
});
