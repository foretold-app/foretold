const graphql = require("graphql");

const resolvers = require('../resolvers');

const iAmOwner = {
  type: graphql.GraphQLNonNull(graphql.GraphQLBoolean),
  resolve: resolvers.common.iAmOwner,
};

const isMe = {
  type: graphql.GraphQLNonNull(graphql.GraphQLBoolean),
  resolve: resolvers.common.isMe,
};

const connectionArguments = {
  after: { type: graphql.GraphQLBoolean },
  before: { type: graphql.GraphQLBoolean },
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

module.exports = {
  iAmOwner,
  isMe,
  connectionArguments,
  pageInfoConnection,
};
