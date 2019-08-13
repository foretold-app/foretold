const index = require('./global-settings');

describe('Global Setting Resolvers', () => {
  it('should be object', () => {
    expect(index).toBeInstanceOf(Object);
  });
});
