const graphql = require("graphql");

const resolvers = require('../resolvers');
const agents = require('./agents');
const channelMemberships = require('./channel-memberhips');

const channel = new graphql.GraphQLObjectType({
  name: 'Channel',
  fields: () => ({
    id: { type: graphql.GraphQLNonNull(graphql.GraphQLString) },
    name: { type: graphql.GraphQLNonNull(graphql.GraphQLString) },
    description: { type: graphql.GraphQLString },
    isArchived: { type: graphql.GraphQLNonNull(graphql.GraphQLBoolean) },
    isPublic: { type: graphql.GraphQLNonNull(graphql.GraphQLBoolean) },
    membershipCount: { type: graphql.GraphQLNonNull(graphql.GraphQLInt) },
    myRole: {
      type: graphql.GraphQLNonNull(require('./channel-memberhips').roleOutput),
      resolve: resolvers.channelMemberships.myRole,
    },

    availableActions: {
      type: graphql.GraphQLList(graphql.GraphQLString),
      resolve: resolvers.permissions.availableChannelMutations,
    },

    creator: {
      type: graphql.GraphQLNonNull(agents.agent),
      resolve: resolvers.channels.channelCreator,
    },

    agents: {
      type: require('../connections').channelAgentsConnection.connectionType,
      args: require('../connections').channelAgentsConnection.connectionArgs,
      resolve: require('../connections').channelAgentsConnection.resolve
    },

    channelMemberships: {
      type: graphql.GraphQLNonNull(graphql.GraphQLList(channelMemberships.channelsMembership)),
      resolve: resolvers.channelMemberships.allByChannelId,
    },
  }),
});

const channelInput = new graphql.GraphQLInputObjectType({
  name: 'ChannelInput',
  fields: () => ({
    name: { type: new graphql.GraphQLNonNull(graphql.GraphQLString) },
    isPublic: { type: graphql.GraphQLNonNull(graphql.GraphQLBoolean) },
    description: { type: graphql.GraphQLString }
  }),
});

module.exports = {
  channel,
  channelInput,
};
