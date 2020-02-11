const graphql = require('graphql');

const resolvers = require('../resolvers');
const agents = require('./agents');
const channelMemberships = require('./channel-memberhips');
const permissionsTypes = require('./permissions');
const commonTypes = require('./common');

const { string3to255 } = require('./scalars');
const { string3to4K } = require('./scalars');
const { string0to32K } = require('./scalars');

const channel = new graphql.GraphQLObjectType({
  name: 'Channel',
  fields: () => ({
    id: { type: graphql.GraphQLNonNull(graphql.GraphQLString) },
    name: { type: graphql.GraphQLNonNull(string3to255) },
    description: { type: string3to4K },
    isArchived: { type: graphql.GraphQLNonNull(graphql.GraphQLBoolean) },
    isPublic: { type: graphql.GraphQLNonNull(graphql.GraphQLBoolean) },
    isCurated: { type: graphql.GraphQLNonNull(graphql.GraphQLBoolean) },
    isBookmarked: { type: graphql.GraphQLBoolean },
    knowledgeGraph: { type: string0to32K },

    membershipCount: {
      type: graphql.GraphQLNonNull(graphql.GraphQLInt),
      resolve: resolvers.channelMemberships.membershipCount,
    },

    myRole: {
      type: graphql.GraphQLNonNull(channelMemberships.roleOutput),
      resolve: resolvers.channelMemberships.myRole,
    },

    permissions: {
      type: graphql.GraphQLNonNull(permissionsTypes.permissions),
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
    name: { type: graphql.GraphQLNonNull(string3to255) },
    isPublic: { type: graphql.GraphQLNonNull(graphql.GraphQLBoolean) },
    isArchived: { type: graphql.GraphQLNonNull(graphql.GraphQLBoolean) },
    description: { type: string3to4K },
    knowledgeGraph: { type: string0to32K },
  }),
});

const orderFieldChannel = new graphql.GraphQLEnumType({
  name: 'OrderFieldChannel',
  values: {
    isCurated: { value: 'isCurated' },
    membersCount: { value: 'membersCount' },
    name: { value: 'name' },
  },
});

const orderChannels = new graphql.GraphQLInputObjectType({
  name: 'OrderChannels',
  fields: () => ({
    field: { type: graphql.GraphQLNonNull(orderFieldChannel) },
    direction: {
      type: graphql.GraphQLNonNull(commonTypes.orderDirection),
    },
  }),
});

module.exports = {
  channel,
  channelInput,
  orderChannels,
};
