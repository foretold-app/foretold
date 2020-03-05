const { UserModel } = require('./user-model');

describe('User Model', () => {
  it('class should be a constructor', () => {
    expect(UserModel).toBeInstanceOf(Function);
  });
});
