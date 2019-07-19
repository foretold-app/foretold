const index = require('./notifications');

describe('Notifications Lib', () => {
  it('returns an object', () => {
    expect(index).toBeInstanceOf(Object);
  });
});
