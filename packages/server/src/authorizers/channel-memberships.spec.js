const channelMemberships = require('./channel-memberships');

describe('Channel Memberships Authorizers', () => {
  it('returns as an object', () => {
    expect(channelMemberships).toBeInstanceOf(Object);
  });
});
