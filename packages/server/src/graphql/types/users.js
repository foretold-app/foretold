const graphql = require('graphql');
const { resolver, DateType } = require('graphql-sequelize');

const models = require('../../models/definitions');

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
    id: { type: graphql.GraphQLNonNull(graphql.GraphQLString) },
    name: { type: graphql.GraphQLNonNull(graphql.GraphQLString) },
    description: { type: graphql.GraphQLString },
    email: { type: graphql.GraphQLString },
    picture: { type: graphql.GraphQLString },
    isEmailVerified: { type: graphql.GraphQLBoolean },
    auth0Id: { type: graphql.GraphQLString },
    createdAt: { type: graphql.GraphQLNonNull(DateType.default) },
    updatedAt: { type: graphql.GraphQLNonNull(DateType.default) },
    agentId: { type: graphql.GraphQLNonNull(graphql.GraphQLString) },
    isMe: require('./common').isMe,

    // @todo: security?
    // @todo: Do not use resolver. Use common interfaces of Data layer.
    agent: {
      type: require('./agents').agent,
      resolve: resolver(models.User.Agent),
    },

    // @todo: security?
    // @todo: Do not use resolver. Use common interfaces of Data layer.
    bots: {
      type: graphql.GraphQLNonNull(graphql.GraphQLList(require('./bots').bot)),
      resolve: resolver(models.User.Bots),
    },
  }),
});

module.exports = {
  user,
  userUpdateInput,
  userAccessTokenUpdateInput,
};
