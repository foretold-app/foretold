const graphql = require('graphql');

const resolvers = require('../resolvers');
const agents = require('./agents');
const channelMemberships = require('./channel-memberhips');

const { string3to255 } = require('./scalars');
const { string3to4K } = require('./scalars');

const channel = new graphql.GraphQLObjectType({
  name: 'Channel',
  fields: () => ({
    id: { type: graphql.GraphQLNonNull(graphql.GraphQLString) },
    name: { type: graphql.GraphQLNonNull(string3to255) },
    description: { type: string3to4K },
    isArchived: { type: graphql.GraphQLNonNull(graphql.GraphQLBoolean) },
    isPublic: { type: graphql.GraphQLNonNull(graphql.GraphQLBoolean) },
    isCurated: { type: graphql.GraphQLNonNull(graphql.GraphQLBoolean) },

    membershipCount: {
      type: graphql.GraphQLNonNull(graphql.GraphQLInt),
      resolve: resolvers.channelMemberships.membershipCount,
    },

    myRole: {
      type: graphql.GraphQLNonNull(require('./channel-memberhips').roleOutput),
      resolve: resolvers.channelMemberships.myRole,
    },

    permissions: {
      type: graphql.GraphQLNonNull(require('./permissions').permissions),
      resolve: resolvers.permissions.channelsPermissions,
    },

    creator: {
      type: graphql.GraphQLNonNull(agents.agent),
      resolve: resolvers.channels.channelCreator,
    },

    channelMemberships: {
      type: graphql.GraphQLNonNull(graphql.GraphQLList(
        channelMemberships.channelsMembership,
      )),
      resolve: resolvers.channelMemberships.allByChannelId,
    },

    openedMeasurablesCount: {
      type: graphql.GraphQLNonNull(graphql.GraphQLInt),
      resolve: resolvers.measurables.openedCount,
    },

    notebooksCount: {
      type: graphql.GraphQLNonNull(graphql.GraphQLInt),
      resolve: resolvers.notebooks.count,
    },
  }),
});

const channelInput = new graphql.GraphQLInputObjectType({
  name: 'ChannelInput',
  fields: () => ({
    name: { type: new graphql.GraphQLNonNull(string3to255) },
    isPublic: { type: graphql.GraphQLNonNull(graphql.GraphQLBoolean) },
    isArchived: { type: graphql.GraphQLNonNull(graphql.GraphQLBoolean) },
    description: { type: string3to4K },
  }),
});

const orderFieldChannel = new graphql.GraphQLEnumType({
  name: 'OrderFieldChannel',
  values: {
    membersCount: { value: 'membersCount' },
    isCurated: { value: 'isCurated' },
  },
});

const orderChannel = new graphql.GraphQLInputObjectType({
  name: 'OrderChannel',
  fields: () => ({
    field: { type: graphql.GraphQLNonNull(orderFieldChannel) },
    direction: {
      type: graphql.GraphQLNonNull(require('./common').orderDirection),
    },
  }),
});

module.exports = {
  channel,
  channelInput,
  orderChannel,
};
