const index = require('./apollo-server');

describe('Apollo Server Provider', () => {
  it('should be an object', () => {
    expect(index).toBeInstanceOf(Object);
  });
});
