const graphql = require('graphql');
const { DateType } = require('graphql-sequelize');

const { FEED_ITEM_BODY } = require('../../enums');
const resolvers = require('../resolvers');

const commonTypes = require('./common');
const channelsTypes = require('./channels');
const agents = require('./agents');
const scalars = require('./scalars');

const feedItemBodyGeneric = new graphql.GraphQLObjectType({
  name: 'FeedItemBodyGeneric',
  fields: () => ({
    item: { type: graphql.GraphQLNonNull(graphql.GraphQLString) },
    description: { type: graphql.GraphQLNonNull(graphql.GraphQLString) },
  }),
});

const feedItemBodyMeasurable = new graphql.GraphQLObjectType({
  name: 'FeedItemBodyMeasurable',
  fields: () => ({
    item: { type: graphql.GraphQLNonNull(graphql.GraphQLString) },
    description: { type: graphql.GraphQLNonNull(graphql.GraphQLString) },
    measurableId: { type: graphql.GraphQLNonNull(graphql.GraphQLString) },
  }),
});

const feedItemBodyMeasurement = new graphql.GraphQLObjectType({
  name: 'FeedItemBodyMeasurement',
  fields: () => ({
    item: { type: graphql.GraphQLNonNull(graphql.GraphQLString) },
    description: { type: graphql.GraphQLNonNull(graphql.GraphQLString) },
    measurableId: { type: graphql.GraphQLNonNull(graphql.GraphQLString) },
    measurementId: { type: graphql.GraphQLNonNull(graphql.GraphQLString) },
  }),
});

const feedItemBodyChannel = new graphql.GraphQLObjectType({
  name: 'FeedItemBodyChannel',
  fields: () => ({
    item: { type: graphql.GraphQLNonNull(graphql.GraphQLString) },
    description: { type: graphql.GraphQLNonNull(graphql.GraphQLString) },
  }),
});

const feedItemBodyNotebook = new graphql.GraphQLObjectType({
  name: 'FeedItemBodyNotebook',
  fields: () => ({
    item: { type: graphql.GraphQLNonNull(graphql.GraphQLString) },
    description: { type: graphql.GraphQLNonNull(graphql.GraphQLString) },
    notebookId: { type: graphql.GraphQLNonNull(graphql.GraphQLString) },
  }),
});

const feedItemBodyJoinedMember = new graphql.GraphQLObjectType({
  name: 'FeedItemBodyJoinedMember',
  fields: () => ({
    item: { type: graphql.GraphQLNonNull(graphql.GraphQLString) },
    description: { type: graphql.GraphQLNonNull(graphql.GraphQLString) },
  }),
});

const feedItemBodySeries = new graphql.GraphQLObjectType({
  name: 'FeedItemBodySeries',
  fields: () => ({
    item: { type: graphql.GraphQLNonNull(graphql.GraphQLString) },
    description: { type: graphql.GraphQLNonNull(graphql.GraphQLString) },
    seriesId: { type: graphql.GraphQLNonNull(graphql.GraphQLString) },
  }),
});

const feedItemBody = new graphql.GraphQLObjectType({
  name: 'FeedItemBody',
  fields: () => ({
    [FEED_ITEM_BODY.generic]: { type: feedItemBodyGeneric },
    [FEED_ITEM_BODY.measurable]: { type: feedItemBodyMeasurable },
    [FEED_ITEM_BODY.measurement]: { type: feedItemBodyMeasurement },
    [FEED_ITEM_BODY.channel]: { type: feedItemBodyChannel },
    [FEED_ITEM_BODY.notebook]: { type: feedItemBodyNotebook },
    [FEED_ITEM_BODY.joinedMember]: { type: feedItemBodyJoinedMember },
    [FEED_ITEM_BODY.series]: { type: feedItemBodySeries },
  }),
});

const feedItem = new graphql.GraphQLObjectType({
  name: 'FeedItem',
  fields: () => ({
    id: { type: graphql.GraphQLNonNull(scalars.$feedItemId) },
    channelId: { type: graphql.GraphQLNonNull(scalars.$channelId) },
    agentId: { type: graphql.GraphQLNonNull(scalars.$agentId) },
    body: { type: graphql.GraphQLNonNull(feedItemBody) },
    createdAt: { type: graphql.GraphQLNonNull(DateType.default) },
    updatedAt: { type: graphql.GraphQLNonNull(DateType.default) },

    agent: {
      type: graphql.GraphQLNonNull(agents.agent),
      resolve: resolvers.agents.one,
    },

    channel: {
      type: graphql.GraphQLNonNull(channelsTypes.channel),
      resolve: resolvers.channels.one,
    },
  }),
});

const feedItemEdge = new graphql.GraphQLObjectType({
  name: 'FeedItemEdge',
  fields: () => ({
    node: { type: feedItem },
    cursor: { type: graphql.GraphQLNonNull(scalars.$cursor) },
  }),
});

const feedItemsConnection = new graphql.GraphQLObjectType({
  name: 'FeedItemsConnection',
  fields: () => ({
    total: { type: graphql.GraphQLInt },
    pageInfo: {
      type: graphql.GraphQLNonNull(commonTypes.pageInfoConnection),
    },
    edges: { type: graphql.GraphQLList(feedItemEdge) },
  }),
});

module.exports = {
  feedItem,
  feedItemEdge,
  feedItemsConnection,
};
