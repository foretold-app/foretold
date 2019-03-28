const graphql = require("graphql");

const roles = new graphql.GraphQLEnumType({
  name: 'AgentChannelRoles',
  values: {
    admin: { value: 'admin' },
    viewer: { value: 'viewer' },
  },
});

const agentsChannel = new graphql.GraphQLObjectType({
  name: 'AgentsChannel',
  fields: {
    agentId: { type: graphql.GraphQLNonNull(graphql.GraphQLString) },
    channelId: { type: graphql.GraphQLNonNull(graphql.GraphQLString) },
    roles: { type: graphql.GraphQLList(roles) },
  },
});

module.exports = {
  agentsChannel,
  roles,
};
