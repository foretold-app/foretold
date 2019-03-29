const graphql = require("graphql");

const role = new graphql.GraphQLEnumType({
  name: 'ChannelMembershipRoles',
  values: {
    ADMIN: { value: 'ADMIN' },
    VIEWER: { value: 'VIEWER' },
  },
});

const channelsMembership = new graphql.GraphQLObjectType({
  name: 'ChannelsMembership',
  fields: {
    agentId: { type: graphql.GraphQLNonNull(graphql.GraphQLString) },
    channelId: { type: graphql.GraphQLNonNull(graphql.GraphQLString) },
    role: { type: graphql.GraphQLNonNull(role) },
  },
});

module.exports = {
  channelsMembership,
  role,
};
