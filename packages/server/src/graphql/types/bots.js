const graphql = require('graphql');
const { DateType } = require('graphql-sequelize');

const resolvers = require('../resolvers');

const bot = new graphql.GraphQLObjectType({
  name: 'Bot',
  fields: () => ({
    id: { type: graphql.GraphQLNonNull(graphql.GraphQLString) },
    name: { type: graphql.GraphQLNonNull(graphql.GraphQLString) },
    description: { type: graphql.GraphQLString },
    picture: { type: graphql.GraphQLString },
    competitorType: {
      type:
      require('./enums/measurement-competitor-type').measurementCompetitorType,
    },
    createdAt: { type: graphql.GraphQLNonNull(DateType.default) },
    updatedAt: { type: graphql.GraphQLNonNull(DateType.default) },
    agentId: { type: graphql.GraphQLString },
    userId: { type: graphql.GraphQLString },
    iAmOwner: require('./common').iAmOwnerByUserId,

    // @todo: security?
    permissions: {
      type: graphql.GraphQLNonNull(require('./permissions').permissions),
      resolve: resolvers.permissions.botsPermissions,
    },

    // @todo: security?
    token: {
      type: graphql.GraphQLString,
      resolve: require('../resolvers/authentications').getTokenByAgentId,
    },

    // @todo: security?
    agent: {
      type: require('./agents').agent,
      resolve: resolvers.agents.one,
    },

    // @todo: security?
    user: {
      type: require('./users').user,
      resolve: resolvers.users.one,
    },
  }),
});

const botInput = new graphql.GraphQLInputObjectType({
  name: 'BotInput',
  fields: () => ({
    name: { type: graphql.GraphQLNonNull(graphql.GraphQLString) },
    description: { type: graphql.GraphQLString },
    picture: { type: graphql.GraphQLString },
    competitorType: {
      type:
      require('./enums/measurement-competitor-type').measurementCompetitorType,
    },
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
    pageInfo: {
      type: graphql.GraphQLNonNull(require('./common').pageInfoConnection),
    },
    edges: { type: graphql.GraphQLList(require('./bots').botsEdge) },
  }),
});

module.exports = {
  bot,
  botInput,
  botsEdge,
  botsConnection,
};
