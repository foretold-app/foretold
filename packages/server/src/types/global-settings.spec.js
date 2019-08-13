const index = require('./global-settings');

describe('Global Settings Type', () => {
  it('should be an object', () => {
    expect(index).toBeInstanceOf(Object);
  });
});
