const graphql = require('graphql');
const { DateType } = require('graphql-sequelize');

const scalars = require('./scalars');

const channelBookmark = new graphql.GraphQLObjectType({
  name: 'ChannelBookmark',
  fields: () => ({
    id: { type: graphql.GraphQLNonNull(scalars.bookmarkId) },
    channelId: { type: graphql.GraphQLNonNull(scalars.channelId) },
    agentId: { type: graphql.GraphQLNonNull(scalars.agentId) },
    createdAt: { type: graphql.GraphQLNonNull(DateType.default) },
    updatedAt: { type: graphql.GraphQLNonNull(DateType.default) },
  }),
});

module.exports = {
  channelBookmark,
};
