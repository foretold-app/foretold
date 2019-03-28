const graphql = require("graphql");

const roles = new graphql.GraphQLEnumType({
  name: 'AgentChannelRoles',
  values: {
    admin: { value: 'admin' },
    viewer: { value: 'viewer' },
    owner: { value: 'owner' },
  },
});

const rolesInput = new graphql.GraphQLEnumType({
  name: 'AgentChannelRolesInput',
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
  },
});

module.exports = {
  agentsChannel,
  roles,
  rolesInput,
};
