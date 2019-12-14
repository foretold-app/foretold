const index = require('./index');

describe('Index', () => {
  it('returns as an object', () => {
    expect(index.Guesstimator).toBeInstanceOf(Object);
  });
});
