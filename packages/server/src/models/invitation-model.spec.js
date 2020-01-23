const { InvitationModel } = require('./invitation-model');

describe('Invitation Model', () => {
  it('class should be a constructor', () => {
    expect(InvitationModel).toBeInstanceOf(Function);
  });
});
