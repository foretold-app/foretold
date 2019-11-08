const index = require('./distributions');

describe('Distributions', () => {
  it('returns as an object', () => {
    expect(index.Distributions).toBeInstanceOf(Object);
  });
});
