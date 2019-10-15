const data = require('./authentication-primary');

describe('Authentication2 Data', () => {
  it('class should be a constructor', () => {
    expect(data.AuthenticationPrimary).toBeInstanceOf(Function);
  });
});
