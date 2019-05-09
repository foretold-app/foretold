const graphql = require("graphql");

const models = require("../models");

const stats = new graphql.GraphQLObjectType({
  name: "Stats",
  fields: {
    agentCount: {
      type: graphql.GraphQLNonNull(graphql.GraphQLInt),
      resolve: async () => {
        return await models.Agent.count();
      }
    },
    userCount: {
      type: graphql.GraphQLNonNull(graphql.GraphQLInt),
      resolve: async () => {
        return await models.User.count();
      }
    },
    botCount: {
      type: graphql.GraphQLNonNull(graphql.GraphQLInt),
      resolve: async () => {
        return await models.Bot.count();
      }
    },
    measurementCount: {
      type: graphql.GraphQLNonNull(graphql.GraphQLInt),
      resolve: async () => {
        return await models.Measurement.count();
      }
    },
    measurableCount: {
      type: graphql.GraphQLNonNull(graphql.GraphQLInt),
      resolve: async () => {
        return await models.Measurable.count();
      }
    }
  }
});

module.exports = {
  stats,
};
