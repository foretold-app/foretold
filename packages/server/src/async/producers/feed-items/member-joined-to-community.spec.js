const { MemberJoinedToCommunity } = require('./member-joined-to-community');

describe('Member Joined To Community Feed Item Producer', () => {
  it('returns an object', () => {
    expect(MemberJoinedToCommunity).toBeInstanceOf(Function);
  });
});
