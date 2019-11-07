const index = require('./filters');

describe('Filters', () => {
  it('returns as an object', () => {
    expect(index.Filters).toBeInstanceOf(Object);
  });
});
