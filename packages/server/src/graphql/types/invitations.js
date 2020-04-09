const graphql = require('graphql');
const { DateType } = require('graphql-sequelize');

const scalars = require('./scalars');

const invitation = new graphql.GraphQLObjectType({
  name: 'Invitation',
  fields: () => ({
    id: { type: graphql.GraphQLNonNull(scalars.$invitationId) },
    channelId: { type: graphql.GraphQLNonNull(scalars.$channelId) },
    inviterAgentId: { type: graphql.GraphQLNonNull(scalars.$agentId) },
    createdAt: { type: graphql.GraphQLNonNull(DateType.default) },
    updatedAt: { type: graphql.GraphQLNonNull(DateType.default) },
  }),
});

const invitationCreateInput = new graphql.GraphQLInputObjectType({
  name: 'InvitationCreateInput',
  fields: () => ({
    email: { type: graphql.GraphQLNonNull(graphql.GraphQLString) },
    channelId: { type: graphql.GraphQLNonNull(scalars.$channelId) },
  }),
});

module.exports = {
  invitationCreateInput,
  invitation,
};
