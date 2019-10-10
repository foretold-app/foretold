const graphql = require('graphql');

const {
  channelMembershipRolesOutput,
} = require('./channel-membership-roles-output');

describe('Channel Membership Roles Output Enum', () => {
  it('channelMembershipRolesOutput', () => {
    expect(channelMembershipRolesOutput)
      .toBeInstanceOf(graphql.GraphQLEnumType);
  });
});
