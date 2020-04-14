const graphql = require('graphql');
const { DateType } = require('graphql-sequelize');

const resolvers = require('../resolvers');
const commonTypes = require('./common');
const botsTypes = require('./bots');
const scalars = require('./scalars');

const userUpdateInput = new graphql.GraphQLInputObjectType({
  name: 'UserUpdateInput',
  fields: () => ({
    name: { type: graphql.GraphQLString },
    email: { type: graphql.GraphQLString },
    picture: { type: graphql.GraphQLString },
    description: { type: graphql.GraphQLString },
  }),
});

const userAccessTokenUpdateInput = new graphql.GraphQLInputObjectType({
  name: 'UserAccessTokenUpdateInput',
  fields: () => ({
    auth0AccessToken: { type: graphql.GraphQLString },
  }),
});

const user = new graphql.GraphQLObjectType({
  name: 'User',
  fields: () => ({
    id: { type: graphql.GraphQLNonNull(scalars.$userId) },
    name: { type: graphql.GraphQLNonNull(graphql.GraphQLString) },
    description: { type: graphql.GraphQLString },
    email: { type: graphql.GraphQLString },
    picture: { type: graphql.GraphQLString },
    isEmailVerified: { type: graphql.GraphQLBoolean },
    auth0Id: { type: graphql.GraphQLString },
    createdAt: { type: graphql.GraphQLNonNull(DateType.default) },
    updatedAt: { type: graphql.GraphQLNonNull(DateType.default) },
    agentId: { type: graphql.GraphQLNonNull(scalars.$agentId) },
    isMe: commonTypes.isMe,

    // @todo: security?
    agent: {
      type: require('./agents').agent,
      resolve: resolvers.agents.one,
    },

    // @todo: security?
    bots: {
      type: graphql.GraphQLNonNull(graphql.GraphQLList(botsTypes.bot)),
      resolve: resolvers.bots.allById,
    },
  }),
});

module.exports = {
  user,
  userUpdateInput,
  userAccessTokenUpdateInput,
};
