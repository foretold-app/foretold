const graphql = require('graphql');
const { resolver, DateType } = require('graphql-sequelize');

const models = require('../models');

const userUpdateInput = new graphql.GraphQLInputObjectType({
  name: 'UserUpdateInput',
  fields: () => ({
    name: { type: graphql.GraphQLNonNull(graphql.GraphQLString) },
    email: { type: graphql.GraphQLString },
    picture: { type: graphql.GraphQLString },
  })
});

const user = new graphql.GraphQLObjectType({
  name: 'User',
  fields: () => ({
    id: { type: graphql.GraphQLNonNull(graphql.GraphQLString) },
    name: { type: graphql.GraphQLNonNull(graphql.GraphQLString) },
    email: { type: graphql.GraphQLString },
    picture: { type: graphql.GraphQLString },
    isEmailVerified: { type: graphql.GraphQLBoolean },
    auth0Id: { type: graphql.GraphQLString },
    createdAt: { type: graphql.GraphQLNonNull(DateType.default) },
    updatedAt: { type: graphql.GraphQLNonNull(DateType.default) },
    agentId: { type: graphql.GraphQLString },
    isMe: require('./common').isMe,

    Agent: {
      type: require('./agents').agent,
      resolve: resolver(models.User.Agent),
    },

    Bots: {
      type: graphql.GraphQLNonNull(graphql.GraphQLList(require('./bots').bot)),
      resolve: resolver(models.User.Bots),
    },
  })
});

module.exports = {
  user,
  userUpdateInput,
};
