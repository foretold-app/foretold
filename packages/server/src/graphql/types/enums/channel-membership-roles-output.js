const graphql = require('graphql');

const {
  CHANNEL_MEMBERSHIP_ROLES,
} = require('../../../enums/channel-membership-roles');

const channelMembershipRolesOutput = new graphql.GraphQLEnumType({
  name: 'ChannelMembershipRolesOutput',
  values: {
    [CHANNEL_MEMBERSHIP_ROLES.ADMIN]: {
      value: CHANNEL_MEMBERSHIP_ROLES.ADMIN,
    },
    [CHANNEL_MEMBERSHIP_ROLES.VIEWER]: {
      value: CHANNEL_MEMBERSHIP_ROLES.VIEWER,
    },
    [CHANNEL_MEMBERSHIP_ROLES.NONE]: {
      value: CHANNEL_MEMBERSHIP_ROLES.NONE,
    },
  },
});

module.exports = {
  channelMembershipRolesOutput,
};
