const graphql = require("graphql");

const resolvers = require('../resolvers');

const { CHANNEL_MEMBERSHIP_ROLES } = require('../models/channel-membership-roles');

const role = new graphql.GraphQLEnumType({
  name: 'ChannelMembershipRoles',
  values: {
    [CHANNEL_MEMBERSHIP_ROLES.ADMIN]: { value: CHANNEL_MEMBERSHIP_ROLES.ADMIN },
    [CHANNEL_MEMBERSHIP_ROLES.VIEWER]: { value: CHANNEL_MEMBERSHIP_ROLES.VIEWER },
  },
});

const roleOutput = new graphql.GraphQLEnumType({
  name: 'ChannelMembershipRolesOutput',
  values: {
    [CHANNEL_MEMBERSHIP_ROLES.ADMIN]: { value: CHANNEL_MEMBERSHIP_ROLES.ADMIN },
    [CHANNEL_MEMBERSHIP_ROLES.VIEWER]: { value: CHANNEL_MEMBERSHIP_ROLES.VIEWER },
    [CHANNEL_MEMBERSHIP_ROLES.NONE]: { value: CHANNEL_MEMBERSHIP_ROLES.NONE },
  },
});

const channelsMembership = new graphql.GraphQLObjectType({
  name: 'ChannelsMembership',
  fields: () => ({
    agentId: { type: graphql.GraphQLNonNull(graphql.GraphQLString) },
    channelId: { type: graphql.GraphQLNonNull(graphql.GraphQLString) },
    role: { type: graphql.GraphQLNonNull(role) },

    permissions: {
      type: graphql.GraphQLNonNull(require('./permissions').permissions),
      resolve: resolvers.permissions.channelMembershipsMutations,
    },

    channel: {
      type: require('./channels').channel,
      resolve: resolvers.channels.one,
    },

    agent: {
      type: require('./agents').agent,
      resolve: resolvers.agents.one,
    },

  }),
});

const channelMembershipRoleInput = new graphql.GraphQLInputObjectType({
  name: 'ChannelMembershipRoleInput',
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

const joiningChannelInput = new graphql.GraphQLInputObjectType({
  name: 'JoiningChannelInput',
  fields: () => ({
    channelId: { type: graphql.GraphQLString },
  }),
});

module.exports = {
  role,
  roleOutput,
  channelsMembership,
  channelMembershipRoleInput,
  channelMembershipDeleteInput,
  joiningChannelInput,
};
