const { MemberJoinedToCommunity } = require('./member-joined-community');

describe('Member Joined Community Feed Item Producer', () => {
  it('returns an object', () => {
    expect(MemberJoinedToCommunity).toBeInstanceOf(Function);
  });
});
