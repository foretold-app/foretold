const index = require('./constructs');

describe('Constructs', () => {
  it('returns as an object', () => {
    expect(index.ImpureConstructs).toBeInstanceOf(Object);
  });
});
