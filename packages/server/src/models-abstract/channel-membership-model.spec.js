const { ChannelMembershipModel } = require('./channel-membership-model');

describe('Channel Membership Model', () => {
  it('class should be a constructor', () => {
    expect(ChannelMembershipModel).toBeInstanceOf(Function);
  });
});
