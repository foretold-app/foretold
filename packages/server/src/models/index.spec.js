const index = require('./index');

describe('Models Abstract', () => {
  it('index should be a an object', () => {
    expect(index).toBeInstanceOf(Object);
  });
});
