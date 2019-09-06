const graphql = require('graphql');

const resolvers = require('../resolvers');

const iAmOwner = {
  type: graphql.GraphQLNonNull(graphql.GraphQLBoolean),
  resolve: resolvers.common.iAmOwner,
};

const iAmOwnerByUserId = {
  type: graphql.GraphQLNonNull(graphql.GraphQLBoolean),
  resolve: resolvers.common.iAmOwnerByUserId,
};

const isMe = {
  type: graphql.GraphQLNonNull(graphql.GraphQLBoolean),
  resolve: resolvers.common.isMe,
};

const connectionArguments = {
  after: { type: graphql.GraphQLString },
  before: { type: graphql.GraphQLString },
  first: { type: graphql.GraphQLInt },
  last: { type: graphql.GraphQLInt },
};

const pageInfoConnection = new graphql.GraphQLObjectType({
  name: 'PageInfoConnection',
  fields: () => ({
    hasNextPage: { type: graphql.GraphQLNonNull(graphql.GraphQLBoolean) },
    hasPreviousPage: { type: graphql.GraphQLNonNull(graphql.GraphQLBoolean) },
    startCursor: { type: graphql.GraphQLString },
    endCursor: { type: graphql.GraphQLString },
  }),
});

const isArchived = new graphql.GraphQLEnumType({
  name: 'isArchived',
  values: {
    TRUE: { value: "TRUE" },
    FALSE: { value: "FALSE" },
  },
});

module.exports = {
  isMe,
  iAmOwner,
  isArchived,
  iAmOwnerByUserId,
  connectionArguments,
  pageInfoConnection,
};
