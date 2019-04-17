const graphql = require('graphql');
const { resolver, DateType } = require('graphql-sequelize');

const models = require('../models');

const bot = new graphql.GraphQLObjectType({
  name: 'Bot',
  fields: () => ({
    id: { type: graphql.GraphQLNonNull(graphql.GraphQLString) },
    name: { type: graphql.GraphQLNonNull(graphql.GraphQLString) },
    description: { type: graphql.GraphQLString },
    competitorType: { type: require('./competitor').competitor },
    createdAt: { type: graphql.GraphQLNonNull(DateType.default) },
    updatedAt: { type: graphql.GraphQLNonNull(DateType.default) },
    agentId: { type: graphql.GraphQLString },
    userId: { type: graphql.GraphQLString },

    jwt: {
      type: graphql.GraphQLString,
      resolve: require('../resolvers/authentications').getJwtByAgentId,
    },

    Agent: {
      type: require('./agents').agent,
      resolve: resolver(models.Bot.Agent),
    },

    User: {
      type: require('./users').user,
      resolve: resolver(models.Bot.User),
    },
  })
});

const botInput = new graphql.GraphQLInputObjectType({
  name: 'BotInput',
  fields: () => ({
    name: { type: graphql.GraphQLNonNull(graphql.GraphQLString) },
    description: { type: graphql.GraphQLString },
    competitorType: { type: require('./competitor').competitor },
  }),
});

module.exports = {
  bot,
  botInput,
};
