const graphql = require('graphql');
const { DateType } = require('graphql-sequelize');

const resolvers = require('../resolvers');

const { measurementCompetitorType } = require('./enums');
const commonTypes = require('./common');
const permissionsTypes = require('./permissions');

const bot = new graphql.GraphQLObjectType({
  name: 'Bot',
  fields: () => ({
    id: { type: graphql.GraphQLNonNull(graphql.GraphQLString) },
    name: { type: graphql.GraphQLNonNull(graphql.GraphQLString) },
    description: { type: graphql.GraphQLString },
    picture: { type: graphql.GraphQLString },
    competitorType: { type: measurementCompetitorType },
    createdAt: { type: graphql.GraphQLNonNull(DateType.default) },
    updatedAt: { type: graphql.GraphQLNonNull(DateType.default) },
    agentId: { type: graphql.GraphQLString },
    userId: { type: graphql.GraphQLString },
    iAmOwner: commonTypes.iAmOwnerByUserId,

    // @todo: security?
    permissions: {
      type: graphql.GraphQLNonNull(permissionsTypes.permissions),
      resolve: resolvers.permissions.botsPermissions,
    },

    // @todo: security?
    token: {
      type: graphql.GraphQLString,
      resolve: resolvers.authentications.getTokenByAgentId,
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
    competitorType: { type: measurementCompetitorType },
  }),
});

const botsEdge = new graphql.GraphQLObjectType({
  name: 'BotsEdge',
  fields: () => ({
    node: { type: bot },
    cursor: { type: graphql.GraphQLNonNull(graphql.GraphQLString) },
  }),
});

const botsConnection = new graphql.GraphQLObjectType({
  name: 'BotsConnection',
  fields: () => ({
    total: { type: graphql.GraphQLInt },
    pageInfo: {
      type: graphql.GraphQLNonNull(commonTypes.pageInfoConnection),
    },
    edges: { type: graphql.GraphQLList(botsEdge) },
  }),
});

module.exports = {
  bot,
  botInput,
  botsEdge,
  botsConnection,
};
