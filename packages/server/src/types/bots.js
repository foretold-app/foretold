const graphql = require('graphql');
const { resolver, DateType } = require('graphql-sequelize');

const models = require('../models');
const resolvers = require('../resolvers');

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
    iAmOwner: require('./common').iAmOwnerByUserId,

    permissions: {
      type: graphql.GraphQLNonNull(require('./permissions').permissions),
      resolve: resolvers.permissions.botsPermissions,
    },

    token: {
      type: graphql.GraphQLString,
      resolve: require('../resolvers/authentications').getTokenByAgentId,
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

const botsEdge = new graphql.GraphQLObjectType({
  name: 'BotsEdge',
  fields: () => ({
    node: { type: require('./bots').bot },
    cursor: { type: graphql.GraphQLNonNull(graphql.GraphQLString) },
  }),
});

const botsConnection = new graphql.GraphQLObjectType({
  name: 'BotsConnection',
  fields: () => ({
    total: { type: graphql.GraphQLInt },
    pageInfo: { type: graphql.GraphQLNonNull(require('./common').pageInfoConnection) },
    edges: { type: graphql.GraphQLList(require('./bots').botsEdge) },
  }),
});

module.exports = {
  bot,
  botInput,
  botsEdge,
  botsConnection,
};
