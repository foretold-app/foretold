const graphql = require('graphql');

const resolvers = require('../resolvers');
const {
  channelMembershipRoles,
} = require('./enums/channel-membership-roles');
const {
  channelMembershipRolesOutput,
} = require('./enums/channel-membership-roles-output');

const { objectId } = require('./scalars');

const channelsMembership = new graphql.GraphQLObjectType({
  name: 'ChannelsMembership',
  fields: () => ({
    agentId: { type: graphql.GraphQLNonNull(objectId) },
    channelId: { type: graphql.GraphQLNonNull(objectId) },
    role: { type: graphql.GraphQLNonNull(channelMembershipRoles) },

    permissions: {
      type: graphql.GraphQLNonNull(require('./permissions').permissions),
      resolve: resolvers.permissions.channelMembershipsPermissions,
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
    agentId: { type: objectId },
    channelId: { type: objectId },
    role: {
      type: graphql.GraphQLNonNull(require('./channel-memberhips').role),
    },
  }),
});

const channelMembershipDeleteInput = new graphql.GraphQLInputObjectType({
  name: 'ChannelMembershipDeleteInput',
  fields: () => ({
    agentId: { type: objectId },
    channelId: { type: objectId },
  }),
});

const joiningChannelInput = new graphql.GraphQLInputObjectType({
  name: 'JoiningChannelInput',
  fields: () => ({
    channelId: { type: objectId },
  }),
});

module.exports = {
  role: channelMembershipRoles,
  roleOutput: channelMembershipRolesOutput,
  channelsMembership,
  channelMembershipRoleInput,
  channelMembershipDeleteInput,
  joiningChannelInput,
};
