const data = require('./authentication2-data');

describe('Authentication2 Data', () => {
  it('class should be a constructor', () => {
    expect(data.AuthenticationData2).toBeInstanceOf(Function);
  });
});
