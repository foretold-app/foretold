const data = require('./authentication');

describe('Authentication Data', () => {
  it('class should be a constructor', () => {
    expect(data.Authentication).toBeInstanceOf(Function);
  });
});
