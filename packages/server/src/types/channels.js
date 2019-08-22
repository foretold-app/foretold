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
    isCurated: { type: graphql.GraphQLNonNull(graphql.GraphQLBoolean) },
    membershipCount: { type: graphql.GraphQLNonNull(graphql.GraphQLInt) },

    myRole: {
      type: graphql.GraphQLNonNull(require('./channel-memberhips').roleOutput),
      resolve: resolvers.channelMemberships.myRole,
    },

    permissions: {
      type: graphql.GraphQLNonNull(require('./permissions').permissions),
      resolve: resolvers.permissions.channelPermissions,
    },

    creator: {
      type: graphql.GraphQLNonNull(agents.agent),
      resolve: resolvers.channels.channelCreator,
    },

    // @todo: security?
    agents: {
      type: require('../connections').channelAgentsConnection.connectionType,
      args: require('../connections').channelAgentsConnection.connectionArgs,
      resolve: require('../connections').channelAgentsConnection.resolve,
    },

    channelMemberships: {
      type: graphql.GraphQLNonNull(graphql.GraphQLList(channelMemberships.channelsMembership)),
      resolve: resolvers.channelMemberships.allByChannelId,
    },

    openedMeasurablesCount: {
      type: graphql.GraphQLNonNull(graphql.GraphQLInt),
      resolve: resolvers.measurables.openedCount,
    },
  }),
});

const channelInput = new graphql.GraphQLInputObjectType({
  name: 'ChannelInput',
  fields: () => ({
    name: { type: new graphql.GraphQLNonNull(graphql.GraphQLString) },
    isPublic: { type: graphql.GraphQLNonNull(graphql.GraphQLBoolean) },
    isArchived: { type: graphql.GraphQLNonNull(graphql.GraphQLBoolean) },
    description: { type: graphql.GraphQLString },
  }),
});

module.exports = {
  channel,
  channelInput,
};
