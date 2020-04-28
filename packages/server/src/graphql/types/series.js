const graphql = require('graphql');
const { DateType } = require('graphql-sequelize');

const resolvers = require('../resolvers');

const commonTypes = require('./common');
const channelsTypes = require('./channels');
const agentsTypes = require('./agents');
const scalars = require('./scalars');

const seriesCreateInput = new graphql.GraphQLInputObjectType({
  name: 'SeriesCreateInput',
  fields: () => ({
    name: { type: graphql.GraphQLString },
    description: { type: graphql.GraphQLString },
    channelId: { type: graphql.GraphQLNonNull(scalars.$channelId) },
    subjects: { type: graphql.GraphQLList(graphql.GraphQLString) },
    properties: { type: graphql.GraphQLList(graphql.GraphQLString) },
    dates: { type: graphql.GraphQLList(DateType.default) },
  }),
});

const seriesUpdateInput = new graphql.GraphQLInputObjectType({
  name: 'SeriesUpdateInput',
  fields: () => ({
    name: { type: graphql.GraphQLString },
    description: { type: graphql.GraphQLString },
    subjects: { type: graphql.GraphQLList(graphql.GraphQLString) },
    properties: { type: graphql.GraphQLList(graphql.GraphQLString) },
    dates: { type: graphql.GraphQLList(DateType.default) },
  }),
});

const series = new graphql.GraphQLObjectType({
  name: 'Series',
  fields: () => ({
    id: { type: graphql.GraphQLNonNull(scalars.$seriesId) },
    name: { type: graphql.GraphQLString },
    description: { type: graphql.GraphQLString },
    subjects: { type: graphql.GraphQLList(graphql.GraphQLString) },
    properties: { type: graphql.GraphQLList(graphql.GraphQLString) },
    dates: { type: graphql.GraphQLList(DateType.default) },
    channelId: { type: graphql.GraphQLNonNull(scalars.$channelId) },

    measurableCount: {
      type: graphql.GraphQLInt,
      resolve: resolvers.measurables.measurableCount,
    },

    createdAt: { type: graphql.GraphQLNonNull(DateType.default) },
    updatedAt: { type: graphql.GraphQLNonNull(DateType.default) },
    creatorId: { type: scalars.$agentId },
    iAmOwner: commonTypes.iAmOwner,

    // @todo: security?
    creator: {
      type: agentsTypes.agent,
      resolve: resolvers.agents.one,
    },

    // @todo: security?
    channel: {
      type: channelsTypes.channel,
      resolve: resolvers.channels.one,
    },
  }),
});

module.exports = {
  series,
  seriesCreateInput,
  seriesUpdateInput,
};
