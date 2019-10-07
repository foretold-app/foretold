const graphql = require('graphql');
const { DateType } = require('graphql-sequelize');

const { objectId } = require('./scalars');

const invitation = new graphql.GraphQLObjectType({
  name: 'Invitation',
  fields: () => ({
    id: { type: graphql.GraphQLNonNull(objectId) },
    channelId: { type: graphql.GraphQLNonNull(objectId) },
    inviterAgentId: { type: graphql.GraphQLNonNull(objectId) },
    createdAt: { type: graphql.GraphQLNonNull(DateType.default) },
    updatedAt: { type: graphql.GraphQLNonNull(DateType.default) },
  }),
});

const invitationCreateInput = new graphql.GraphQLInputObjectType({
  name: 'InvitationCreateInput',
  fields: () => ({
    email: { type: graphql.GraphQLNonNull(graphql.GraphQLString) },
    channelId: { type: graphql.GraphQLNonNull(objectId) },
  }),
});

module.exports = {
  invitationCreateInput,
  invitation,
};
