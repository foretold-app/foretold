const data = require('./authentication-data');

describe('Authentication Data', () => {
  it('class should be a constructor', () => {
    expect(data.AuthenticationData).toBeInstanceOf(Function);
  });
});
