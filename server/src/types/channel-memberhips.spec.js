const graphql = require("graphql");

const channelMemberships = require('./channel-memberhips');

describe('channels memberships types', () => {
  it('class should be object', () => {
    expect(channelMemberships).toBeInstanceOf(Object);
  });
  it('channelsMembership', () => {
    expect(channelMemberships.channelsMembership).toBeInstanceOf(graphql.GraphQLObjectType);
  });
});
