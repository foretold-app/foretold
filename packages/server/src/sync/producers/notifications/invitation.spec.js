const { Invitation } = require('./invitation');

describe('Producers Notifications', () => {
  it('returns a construction', () => {
    expect(Invitation).toBeInstanceOf(Function);
  });
});
