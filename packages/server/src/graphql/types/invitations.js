const graphql = require('graphql');
const { DateType } = require('graphql-sequelize');

const { objectId, string256 } = require('./scalars');

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
    email: { type: graphql.GraphQLNonNull(string256) },
    channelId: { type: graphql.GraphQLNonNull(objectId) },
  }),
});

module.exports = {
  invitationCreateInput,
  invitation,
};
