const index = require('./jwt');

describe('JWT Lib', () => {
  it('returns an object', () => {
    expect(index).toBeInstanceOf(Object);
  });
});
