const graphql = require("graphql");

const resolvers = require('../resolvers');

const iAmOwner = {
  type: graphql.GraphQLNonNull(graphql.GraphQLString),
  resolve: resolvers.common.iAmOwner,
};

module.exports = {
  iAmOwner,
};
