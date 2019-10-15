const { UserUpdater } = require('./user-updater');

describe('UserUpdater Class', () => {
  it('returns a constructor', () => {
    expect(UserUpdater).toBeInstanceOf(Function);
  });
});
