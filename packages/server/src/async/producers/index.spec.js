const index = require('./');

describe('Producers Module', () => {
  it('returns an constructor', () => {
    expect(index).toBeInstanceOf(Object);
  });
});
