const graphql = require('graphql');
const { resolver, DateType } = require('graphql-sequelize');

const models = require('../../models');
const { string64, string512 } = require('./scalars');

const userUpdateInput = new graphql.GraphQLInputObjectType({
  name: 'UserUpdateInput',
  fields: () => ({
    name: { type: graphql.GraphQLNonNull(graphql.GraphQLString) },
    email: { type: string64 },
    picture: { type: string512 },
    description: { type: string512 },
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

    // security?
    agent: {
      type: require('./agents').agent,
      resolve: resolver(models.User.Agent),
    },

    // security?
    bots: {
      type: graphql.GraphQLNonNull(graphql.GraphQLList(require('./bots').bot)),
      resolve: resolver(models.User.Bots),
    },
  }),
});

module.exports = {
  user,
  userUpdateInput,
};
