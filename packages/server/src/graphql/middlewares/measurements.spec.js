const index = require('./measurements');

describe('Measurements Middleware', () => {
  it('returns an object', () => {
    expect(index).toBeInstanceOf(Object);
  });
});
