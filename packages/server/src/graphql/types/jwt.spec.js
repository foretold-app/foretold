const jwt = require('./jwt');

describe('JWT Type', () => {
  it('should be an object', () => {
    expect(jwt).toBeInstanceOf(Object);
  });
});
