const graphql = require('graphql');

const channelMemberships = require('./channel-memberhips');

describe('channels memberships types', () => {
  it('class should be object', () => {
    expect(channelMemberships).toBeInstanceOf(Object);
  });
  it('channelsMembership', () => {
    expect(channelMemberships.channelsMembership)
      .toBeInstanceOf(graphql.GraphQLObjectType);
  });
  it('role is an enum', () => {
    expect(channelMemberships.role)
      .toBeInstanceOf(graphql.GraphQLEnumType);
  });
  it('roleOutput is an enum', () => {
    expect(channelMemberships.roleOutput)
      .toBeInstanceOf(graphql.GraphQLEnumType);
  });
  it('channelMembershipRoleInput is an input type', () => {
    expect(channelMemberships.channelMembershipRoleInput)
      .toBeInstanceOf(graphql.GraphQLInputObjectType);
  });
  it('channelMembershipDeleteInput is an input type', () => {
    expect(channelMemberships.channelMembershipDeleteInput)
      .toBeInstanceOf(graphql.GraphQLInputObjectType);
  });
  it('joiningChannelInput is an input type', () => {
    expect(channelMemberships.joiningChannelInput)
      .toBeInstanceOf(graphql.GraphQLInputObjectType);
  });
});
