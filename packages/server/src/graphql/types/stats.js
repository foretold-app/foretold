const graphql = require('graphql');

const models = require('../../models');

/**
 * @todo: use resolvers! never use models this way.
 */
const stats = new graphql.GraphQLObjectType({
  name: 'Stats',
  fields: {
    agentCount: {
      type: graphql.GraphQLNonNull(graphql.GraphQLInt),
      resolve: async () => models.Agent.count(),
    },
    userCount: {
      type: graphql.GraphQLNonNull(graphql.GraphQLInt),
      resolve: async () => models.User.count(),
    },
    botCount: {
      type: graphql.GraphQLNonNull(graphql.GraphQLInt),
      resolve: async () => models.Bot.count(),
    },
    measurementCount: {
      type: graphql.GraphQLNonNull(graphql.GraphQLInt),
      resolve: async () => models.Measurement.count(),
    },
    measurableCount: {
      type: graphql.GraphQLNonNull(graphql.GraphQLInt),
      resolve: async () => models.Measurable.count(),
    },
  },
});

module.exports = {
  stats,
};
