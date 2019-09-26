const graphql = require('graphql');
const { DateType } = require('graphql-sequelize');

const invitation = new graphql.GraphQLObjectType({
  name: 'Invitation',
  fields: () => ({
    id: { type: graphql.GraphQLNonNull(graphql.GraphQLString) },
    agentId: { type: graphql.GraphQLNonNull(graphql.GraphQLString) },
    channelId: { type: graphql.GraphQLNonNull(graphql.GraphQLString) },
    email: { type: graphql.GraphQLNonNull(graphql.GraphQLString) },
    inviterAgentId: { type: graphql.GraphQLNonNull(graphql.GraphQLString) },
    createdAt: { type: graphql.GraphQLNonNull(DateType.default) },
    updatedAt: { type: graphql.GraphQLNonNull(DateType.default) },
  }),
});

const invitationCreateInput = new graphql.GraphQLInputObjectType({
  name: 'InvitationCreateInput',
  fields: () => ({
    email: { type: graphql.GraphQLNonNull(graphql.GraphQLString) },
    channelId: { type: graphql.GraphQLNonNull(graphql.GraphQLString) },
  }),
});

module.exports = {
  invitationCreateInput,
  invitation,
};
