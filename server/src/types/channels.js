const graphql = require("graphql");

const resolvers = require('../resolvers');
const agents = require('./agents');

const channel = new graphql.GraphQLObjectType({
  name: 'Channel',
  fields: {
    id: { type: graphql.GraphQLNonNull(graphql.GraphQLString) },
    name: { type: graphql.GraphQLNonNull(graphql.GraphQLString) },
    description: { type: graphql.GraphQLString },
    isArchived: { type: graphql.GraphQLNonNull(graphql.GraphQLBoolean) },
    isPublic: { type: graphql.GraphQLNonNull(graphql.GraphQLBoolean) },
    creator: {
      type: graphql.GraphQLNonNull(agents.agent2),
      resolve: resolvers.channels.channelCreator,
    },
    agents: {
      type: graphql.GraphQLNonNull(graphql.GraphQLList(agents.agent2)),
      resolve: resolvers.channels.channelAgents,
    },
  }
});

const channelInput = new graphql.GraphQLInputObjectType({
  name: 'ChannelInput',
  fields: {
    name: { type: new graphql.GraphQLNonNull(graphql.GraphQLString) },
    isPublic: { type: graphql.GraphQLNonNull(graphql.GraphQLBoolean) },
    description: { type: graphql.GraphQLString }
  }
});

module.exports = {
  channel,
  channelInput
};
