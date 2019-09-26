const graphql = require('graphql');
const { DateType } = require('graphql-sequelize');

const agentChannel = new graphql.GraphQLObjectType({
  name: 'AgentChannel',
  fields: () => ({
    id: { type: graphql.GraphQLNonNull(graphql.GraphQLString) },
    agentId: { type: graphql.GraphQLNonNull(graphql.GraphQLString) },
    channelId: { type: graphql.GraphQLNonNull(graphql.GraphQLString) },
    primaryPointScore: {
      type: graphql.GraphQLFloat,
      resolve: require('../resolvers').agentChannels.primaryPointScore,
    },
    numberOfPredictions: { type: graphql.GraphQLNonNull(graphql.GraphQLInt) },
    numberOfQuestionsScored: { type: graphql.GraphQLNonNull(graphql.GraphQLInt) },
    createdAt: { type: graphql.GraphQLNonNull(DateType.default) },
    updatedAt: { type: graphql.GraphQLNonNull(DateType.default) },

    // OK
    agent: {
      type: graphql.GraphQLNonNull(require('./agents').agent),
      resolve: require('../resolvers').agents.one,
    },

    // OK
    channel: {
      type: graphql.GraphQLNonNull(require('./channels').channel),
      resolve: require('../resolvers').channels.one,
    },
  }),
});

const agentChannelsEdge = new graphql.GraphQLObjectType({
  name: 'AgentChannelsEdge',
  fields: () => ({
    node: { type: require('./agent-channels').agentChannel },
    cursor: { type: graphql.GraphQLNonNull(graphql.GraphQLString) },
  }),
});

const agentChannelsConnection = new graphql.GraphQLObjectType({
  name: 'AgentChannelsConnection',
  fields: () => ({
    total: { type: graphql.GraphQLInt },
    pageInfo: { type: graphql.GraphQLNonNull(require('./common').pageInfoConnection) },
    edges: { type: graphql.GraphQLList(require('./agent-channels').agentChannelsEdge) },
  }),
});

module.exports = {
  agentChannel,
  agentChannelsEdge,
  agentChannelsConnection,
};
