const graphql = require('graphql');

const resolvers = require('../resolvers');

const stats = new graphql.GraphQLObjectType({
  name: 'Stats',
  fields: {
    agentCount: {
      type: graphql.GraphQLNonNull(graphql.GraphQLInt),
      resolve: resolvers.agents.count,
    },
    userCount: {
      type: graphql.GraphQLNonNull(graphql.GraphQLInt),
      resolve: resolvers.users.count,
    },
    botCount: {
      type: graphql.GraphQLNonNull(graphql.GraphQLInt),
      resolve: resolvers.bots.count,
    },
    measurementCount: {
      type: graphql.GraphQLNonNull(graphql.GraphQLInt),
      resolve: resolvers.measurements.count,
    },
    measurableCount: {
      type: graphql.GraphQLNonNull(graphql.GraphQLInt),
      resolve: resolvers.measurables.count,
    },
  },
});

module.exports = {
  stats,
};
