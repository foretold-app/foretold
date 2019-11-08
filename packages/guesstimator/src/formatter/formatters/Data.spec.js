const index = require('./Data');

describe('Data', () => {
  it('returns as an object', () => {
    expect(index.item).toBeInstanceOf(Object);
  });
});
