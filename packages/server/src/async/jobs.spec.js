const index = require('./jobs');

describe('Jobs Async', () => {
  it('returns an object', () => {
    expect(index).toBeInstanceOf(Object);
  });
});
