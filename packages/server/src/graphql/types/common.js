const graphql = require('graphql');

const resolvers = require('../resolvers');
const { cursor, int500 } = require('./scalars');

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
  after: { type: cursor },
  before: { type: cursor },
  first: { type: int500 },
  last: { type: int500 },
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
    TRUE: { value: 'TRUE' },
    FALSE: { value: 'FALSE' },
  },
});

const orderDirection = new graphql.GraphQLEnumType({
  name: 'OrderDirection',
  values: {
    ASC: { value: 'ASC' },
    DESC: { value: 'DESC' },
  },
});

module.exports = {
  connectionArguments,
  iAmOwner,
  iAmOwnerByUserId,
  isArchived,
  isMe,
  orderDirection,
  pageInfoConnection,
};
