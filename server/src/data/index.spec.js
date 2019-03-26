const index = require('./index');

describe('data index', () => {
  it('index should be an object', () => {
    expect(index).toBeInstanceOf(Object);
  });
});
