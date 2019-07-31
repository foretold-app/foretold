const { InvitationsData } = require('./invitations-data');

describe('Invitation Data', () => {
  it('class should be a constructor', () => {
    expect(InvitationsData).toBeInstanceOf(Function);
  });
});
