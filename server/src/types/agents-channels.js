const graphql = require("graphql");

const agentsChannel = new graphql.GraphQLObjectType({
  name: 'AgentsChannel',
  fields: {
    agentId: { type: graphql.GraphQLString },
    channelId: { type: graphql.GraphQLString },
  }
});

module.exports = {
  agentsChannel,
};
