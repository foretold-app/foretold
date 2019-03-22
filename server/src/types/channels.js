const graphql = require("graphql");

const resolvers = require('../resolvers');
const agents = require('./agents');

const channel = new graphql.GraphQLObjectType({
  name: 'Channel',
  fields: {
    id: { type: graphql.GraphQLString },
    name: { type: graphql.GraphQLString },
    description: { type: graphql.GraphQLString },
    isArchived: { type: graphql.GraphQLBoolean },
    isPublic: { type: graphql.GraphQLBoolean },
    creator: {
      type: agents.agent2,
      resolve: resolvers.channels.channelCreator,
    },
    agents: {
      type: graphql.GraphQLList(agents.agent2),
      resolve: resolvers.channels.channelAgents,
    },
  }
});

const channelInput = new graphql.GraphQLInputObjectType({
  name: 'ChannelInput',
  fields: {
    name: { type: new graphql.GraphQLNonNull(graphql.GraphQLString) },
    description: { type: graphql.GraphQLString },
    isArchived: { type: graphql.GraphQLBoolean },
    isPublic: { type: graphql.GraphQLBoolean },
  }
});

module.exports = {
  channel,
  channelInput
};
