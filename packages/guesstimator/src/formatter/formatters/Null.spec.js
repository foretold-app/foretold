const index = require('./Null');

describe('Lib', () => {
  it('returns as an object', () => {
    expect(index.item).toBeInstanceOf(Object);
  });
});
