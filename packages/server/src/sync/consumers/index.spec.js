const index = require('./');

describe('Consumers Module', () => {
  it('returns an constructor', () => {
    expect(index).toBeInstanceOf(Object);
  });
});
