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
    creator: {
      type: graphql.GraphQLNonNull(agents.agent),
      resolve: resolvers.channels.channelCreator,
    },
    agents: {
      type: graphql.GraphQLNonNull(graphql.GraphQLList(agents.agent)),
      resolve: resolvers.channels.channelAgents,
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

const channelMembershipRoleCreateInput = new graphql.GraphQLInputObjectType({
  name: 'ChannelMembershipRoleCreateInput',
  fields: () => ({
    agentId: { type: graphql.GraphQLString },
    channelId: { type: graphql.GraphQLString },
    role: { type: graphql.GraphQLNonNull(require('./channel-memberhips').role) },
  }),
});

const channelMembershipRoleUpdateInput = new graphql.GraphQLInputObjectType({
  name: 'ChannelMembershipRoleUpdateInput',
  fields: () => ({
    agentId: { type: graphql.GraphQLString },
    channelId: { type: graphql.GraphQLString },
    role: { type: graphql.GraphQLNonNull(require('./channel-memberhips').role) },
  }),
});

const channelMembershipDeleteInput = new graphql.GraphQLInputObjectType({
  name: 'ChannelMembershipDeleteInput',
  fields: () => ({
    agentId: { type: graphql.GraphQLString },
    channelId: { type: graphql.GraphQLString },
  }),
});

module.exports = {
  channel,
  channelInput,
  channelMembershipRoleCreateInput,
  channelMembershipRoleUpdateInput,
  channelMembershipDeleteInput,
};
