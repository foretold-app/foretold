const permissions = require('./permissions');

describe('Permissions Types', () => {
  it('returns as an object', () => {
    expect(permissions).toBeInstanceOf(Object);
  });
});
