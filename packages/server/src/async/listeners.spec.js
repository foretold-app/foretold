const index = require('./listeners');

describe('Listeners Async', () => {
  it('returns an object', () => {
    expect(index).toBeInstanceOf(Object);
  });
});
