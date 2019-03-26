const index = require('./index');

describe('tests authorizes module as index.js file', () => {
  it('should return index as an object', () => {
    expect(index).toBeInstanceOf(Object);
  });
});
