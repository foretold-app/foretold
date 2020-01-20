const notebooks = require('./notebooks');

describe('Notebooks Authorizers', () => {
  it('returns as an object', () => {
    expect(notebooks).toBeInstanceOf(Object);
  });
});
