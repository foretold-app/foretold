const graphql = require("graphql");

const resolvers = require('../resolvers');

const role = new graphql.GraphQLEnumType({
  name: 'ChannelMembershipRoles',
  values: {
    ADMIN: { value: 'ADMIN' },
    VIEWER: { value: 'VIEWER' },
  },
});

const channelsMembership = new graphql.GraphQLObjectType({
  name: 'ChannelsMembership',
  fields: () => ({
    agentId: { type: graphql.GraphQLNonNull(graphql.GraphQLString) },
    channelId: { type: graphql.GraphQLNonNull(graphql.GraphQLString) },
    role: { type: graphql.GraphQLNonNull(role) },
    channel: {
      type: require('./channels').channel,
      resolve: resolvers.channels.one,
    },
    agent: {
      type: require('./agents').agent,
      resolve: resolvers.agents.one,
    },
  }),
});

module.exports = {
  channelsMembership,
  role,
};
