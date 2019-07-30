const invitations = require('./invitations');

describe('Invitations Types', () => {
  it('should be an object', () => {
    expect(invitations).toBeInstanceOf(Object);
  });
});
