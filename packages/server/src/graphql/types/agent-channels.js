const graphql = require('graphql');
const { DateType } = require('graphql-sequelize');
const resolvers = require('../resolvers');

const agentsTypes = require('./agents');
const channelsTypes = require('./channels');
const commonTypes = require('./common');

const agentChannel = new graphql.GraphQLObjectType({
  name: 'AgentChannel',
  fields: () => ({
    id: { type: graphql.GraphQLNonNull(graphql.GraphQLString) },
    agentId: { type: graphql.GraphQLNonNull(graphql.GraphQLString) },
    channelId: { type: graphql.GraphQLNonNull(graphql.GraphQLString) },
    primaryPointScore: {
      type: graphql.GraphQLFloat,
      resolve: resolvers.agentChannels.primaryPointScore,
    },
    numberOfPredictions: { type: graphql.GraphQLNonNull(graphql.GraphQLInt) },
    numberOfQuestionsScored: {
      type: graphql.GraphQLNonNull(graphql.GraphQLInt),
    },
    totalVotes: { type: graphql.GraphQLInt },
    createdAt: { type: graphql.GraphQLNonNull(DateType.default) },
    updatedAt: { type: graphql.GraphQLNonNull(DateType.default) },

    // OK
    agent: {
      type: graphql.GraphQLNonNull(agentsTypes.agent),
      resolve: resolvers.agents.one,
    },

    // OK
    channel: {
      type: graphql.GraphQLNonNull(channelsTypes.channel),
      resolve: resolvers.channels.one,
    },
  }),
});

const agentChannelsEdge = new graphql.GraphQLObjectType({
  name: 'AgentChannelsEdge',
  fields: () => ({
    node: { type: agentChannel },
    cursor: { type: graphql.GraphQLNonNull(graphql.GraphQLString) },
  }),
});

const agentChannelsConnection = new graphql.GraphQLObjectType({
  name: 'AgentChannelsConnection',
  fields: () => ({
    total: { type: graphql.GraphQLInt },
    pageInfo: {
      type: graphql.GraphQLNonNull(commonTypes.pageInfoConnection),
    },
    edges: { type: graphql.GraphQLList(agentChannelsEdge) },
  }),
});

module.exports = {
  agentChannel,
  agentChannelsEdge,
  agentChannelsConnection,
};
