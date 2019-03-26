const graphql = require("graphql");

const agentsChannel = new graphql.GraphQLObjectType({
  name: 'AgentsChannel',
  fields: {
    agentId: { type: graphql.GraphQLNonNull(graphql.GraphQLString) },
    channelId: { type: graphql.GraphQLNonNull(graphql.GraphQLString) },
  },
});

module.exports = {
  agentsChannel,
};
