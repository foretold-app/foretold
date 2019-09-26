const graphql = require('graphql');
const { DateType, resolver } = require('graphql-sequelize');

const models = require('../../models');

const { FEED_ITEM_BODY } = require('../../enums/feed-item-body');

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

const feedItemBody = new graphql.GraphQLObjectType({
  name: 'FeedItemBody',
  fields: () => ({
    [FEED_ITEM_BODY.generic]: { type: feedItemBodyGeneric },
    [FEED_ITEM_BODY.measurable]: { type: feedItemBodyMeasurable },
  }),
});

const feedItem = new graphql.GraphQLObjectType({
  name: 'FeedItem',
  fields: () => ({
    id: { type: graphql.GraphQLNonNull(graphql.GraphQLString) },
    channelId: { type: graphql.GraphQLNonNull(graphql.GraphQLString) },
    body: { type: graphql.GraphQLNonNull(feedItemBody) },
    createdAt: { type: graphql.GraphQLNonNull(DateType.default) },
    updatedAt: { type: graphql.GraphQLNonNull(DateType.default) },

    Channel: {
      type: graphql.GraphQLNonNull(require('./channels').channel),
      resolve: resolver(models.FeedItem.Channel),
    },
  }),
});

const feedItemEdge = new graphql.GraphQLObjectType({
  name: 'FeedItemEdge',
  fields: () => ({
    node: { type: require('./feed-items').feedItem },
    cursor: { type: graphql.GraphQLNonNull(graphql.GraphQLString) },
  }),
});

const feedItemsConnection = new graphql.GraphQLObjectType({
  name: 'FeedItemsConnection',
  fields: () => ({
    total: { type: graphql.GraphQLInt },
    pageInfo: { type: graphql.GraphQLNonNull(require('./common').pageInfoConnection) },
    edges: { type: graphql.GraphQLList(require('./feed-items').feedItemEdge) },
  }),
});

module.exports = {
  feedItem,
  feedItemEdge,
  feedItemsConnection,
};
