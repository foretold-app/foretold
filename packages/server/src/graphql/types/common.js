const graphql = require('graphql');
const { GraphQLInputInt } = require('graphql-input-number');

const resolvers = require('../resolvers');
const { cursor } = require('./scalars');

const int500 = GraphQLInputInt({
  name: 'Int500',
  min: 1,
  max: 500,
});

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

const orderField = new graphql.GraphQLEnumType({
  name: 'OrderField',
  values: {
    primaryPointScore: { value: 'primaryPointScore' },
  },
});

const orderDirection = new graphql.GraphQLEnumType({
  name: 'OrderDirection',
  values: {
    ASC: { value: 'ASC' },
    DESC: { value: 'DESC' },
  },
});

const order = new graphql.GraphQLInputObjectType({
  name: 'Order',
  fields: () => ({
    field: { type: graphql.GraphQLNonNull(orderField) },
    direction: { type: graphql.GraphQLNonNull(orderDirection) },
  }),
});

const orderList = new graphql.GraphQLList(order);

module.exports = {
  isMe,
  orderList,
  iAmOwner,
  isArchived,
  iAmOwnerByUserId,
  connectionArguments,
  pageInfoConnection,

  int500,
};
