const index = require('./index');

describe('tests data module', () => {
  it('index should be an object', () => {
    expect(index).toBeInstanceOf(Object);
  });
});
