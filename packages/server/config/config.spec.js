const index = require('./config');

describe('Config', () => {
  it('returns as an object', () => {
    expect(index).toBeInstanceOf(Object);
  });
});
