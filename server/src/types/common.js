const graphql = require("graphql");

const resolvers = require('../resolvers');

const iAmOwner = {
  type: graphql.GraphQLNonNull(graphql.GraphQLString),
  resolve: resolvers.common.iAmOwner,
};

const isMe = {
  type: graphql.GraphQLNonNull(graphql.GraphQLString),
  resolve: resolvers.common.isMe,
};

module.exports = {
  iAmOwner,
  isMe,
};
