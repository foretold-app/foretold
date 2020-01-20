const index = require('./notebooks');

describe('Notebooks Middleware', () => {
  it('returns an object', () => {
    expect(index).toBeInstanceOf(Object);
  });
});
