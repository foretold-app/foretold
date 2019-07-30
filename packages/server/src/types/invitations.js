const graphql = require('graphql');

const invitationCreateInput = new graphql.GraphQLInputObjectType({
  name: 'InvitationCreateInput',
  fields: () => ({
    email: { type: graphql.GraphQLNonNull(graphql.GraphQLString) },
    channelId: { type: graphql.GraphQLNonNull(graphql.GraphQLString) },
  })
});

module.exports = {
  invitationCreateInput,
};
