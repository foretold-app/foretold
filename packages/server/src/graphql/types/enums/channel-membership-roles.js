const graphql = require('graphql');

const { CHANNEL_MEMBERSHIP_ROLES } = require('../../../models/enums/channel-membership-roles');

const channelMembershipRoles = new graphql.GraphQLEnumType({
  name: 'ChannelMembershipRoles',
  values: {
    [CHANNEL_MEMBERSHIP_ROLES.ADMIN]: { value: CHANNEL_MEMBERSHIP_ROLES.ADMIN },
    [CHANNEL_MEMBERSHIP_ROLES.VIEWER]: { value: CHANNEL_MEMBERSHIP_ROLES.VIEWER },
  },
});

module.exports = {
  channelMembershipRoles,
};
