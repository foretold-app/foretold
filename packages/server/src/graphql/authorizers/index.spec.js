const index = require('./index');

describe('authorizes module as index.js file', () => {
  it('returns index as an object', () => {
    expect(index).toBeInstanceOf(Object);
  });
});
