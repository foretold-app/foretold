const graphql = require("graphql");

const resolvers = require('../resolvers');

const { agent2 } = require('./agent2');

const channel = new graphql.GraphQLObjectType({
  name: 'Channel',
  fields: {
    id: { type: graphql.GraphQLString },
    name: { type: graphql.GraphQLString },
    description: { type: graphql.GraphQLString },
    isArchived: { type: graphql.GraphQLBoolean },
    isPublic: { type: graphql.GraphQLBoolean },
    creator: {
      type: agent2,
      resolve: resolvers.channelCreator,
    },
    agents: {
      type: graphql.GraphQLList(agent2),
      resolve: resolvers.channelAgents,
    },
  }
});

module.exports = {
  channel
};
