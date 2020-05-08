const index = require('./db-config');

describe('DB Configuration', () => {
  it('returns an object', () => {
    expect(index).toBeInstanceOf(Object);
  });
});
