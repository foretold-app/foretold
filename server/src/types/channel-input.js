const graphql = require("graphql");

const channelInput = new graphql.GraphQLInputObjectType({
  name: 'ChannelInput',
  fields: {
    name: { type: new graphql.GraphQLNonNull(graphql.GraphQLString) },
    description: { type: graphql.GraphQLString },
    isArchived: { type: graphql.GraphQLBoolean },
    isPublic: { type: graphql.GraphQLBoolean },
  }
});

module.exports = {
  channelInput
};
