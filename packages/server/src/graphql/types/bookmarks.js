const graphql = require('graphql');
const { DateType } = require('graphql-sequelize');

const scalars = require('./scalars');

// @todo: Not-null
const bookmark = new graphql.GraphQLObjectType({
  name: 'Bookmark',
  fields: () => ({
    id: { type: graphql.GraphQLNonNull(scalars.bookmarkId) },
    channelId: { type: scalars.channelId },
    agentId: { type: graphql.GraphQLNonNull(scalars.agentId) },
    notebookId: { type: scalars.notebookId },
    createdAt: { type: graphql.GraphQLNonNull(DateType.default) },
    updatedAt: { type: graphql.GraphQLNonNull(DateType.default) },
  }),
});

// @todo: Not-null
const bookmarkInput = new graphql.GraphQLInputObjectType({
  name: 'BookmarkInput',
  fields: () => ({
    channelId: { type: scalars.channelId },
    notebookId: { type: scalars.notebookId },
  }),
});

module.exports = {
  bookmark,
  bookmarkInput,
};
