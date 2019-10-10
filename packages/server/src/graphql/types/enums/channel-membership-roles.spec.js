const graphql = require('graphql');

const { channelMembershipRoles } = require('./channel-membership-roles');

describe('Channel Membership Roles Enum', () => {
  it('channelMembershipRoles', () => {
    expect(channelMembershipRoles)
      .toBeInstanceOf(graphql.GraphQLEnumType);
  });
});
