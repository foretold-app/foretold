const data = require('./authentication-primary-data');

describe('Authentication2 Data', () => {
  it('class should be a constructor', () => {
    expect(data.AuthenticationPrimaryData).toBeInstanceOf(Function);
  });
});
