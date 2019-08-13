const index = require('./data');

describe('Data', () => {
  it('returns an object', () => {
    expect(index).toBeInstanceOf(Object);
  });
});
