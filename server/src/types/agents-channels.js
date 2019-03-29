const graphql = require("graphql");

const role = new graphql.GraphQLEnumType({
  name: 'AgentChannelRoles',
  values: {
    ADMIN: { value: 'ADMIN' },
    VIEWER: { value: 'VIEWER' },
  },
});

const agentsChannel = new graphql.GraphQLObjectType({
  name: 'AgentsChannel',
  fields: {
    agentId: { type: graphql.GraphQLNonNull(graphql.GraphQLString) },
    channelId: { type: graphql.GraphQLNonNull(graphql.GraphQLString) },
    role: { type: graphql.GraphQLNonNull(role) },
  },
});

module.exports = {
  agentsChannel,
  role,
};
