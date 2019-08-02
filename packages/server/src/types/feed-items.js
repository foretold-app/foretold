const graphql = require('graphql');
const { DateType } = require('graphql-sequelize');

const { FEED_ITEM_BODY } = require('../models/enums/feed-item-body');

const feedItemBodyCommon = new graphql.GraphQLObjectType({
  name: 'FeedItemBodyCommon',
  fields: () => ({
    item: { type: graphql.GraphQLNonNull(graphql.GraphQLString) },
    description: { type: graphql.GraphQLNonNull(graphql.GraphQLString) },
  }),
});

const feedItemBody = new graphql.GraphQLObjectType({
  name: 'FeedItemBody',
  fields: () => ({
    [FEED_ITEM_BODY.common]: { type: feedItemBodyCommon },
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
  })
});

module.exports = {
  feedItem,
};
