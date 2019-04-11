const _ = require('lodash');
const { shield, allow, and, or } = require('graphql-shield');

const { isAuthenticated } = require('./users');
const { isAdmin, isViewer } = require('./channel-memberships');
const { isChannelPublic } = require('./channels');
const { isOwner } = require('./measurables');

const rulesChannel = {
  Mutation: {
    channelUpdate: and(isAuthenticated, isAdmin),
    leaveChannel: and(isAuthenticated),
    joinChannel: and(isAuthenticated, isChannelPublic),
    channelMembershipCreate: and(isAuthenticated, isAdmin),
  }
};

const rulesChannelMemberships = {
  Mutation: {
    channelMembershipDelete: and(isAuthenticated, isAdmin),
    channelMembershipRoleUpdate: and(isAuthenticated, isAdmin),
  }
};

const rules = {
  Query: {
    '*': allow,
    permissions: allow,
    user: allow,
    users: allow,
    measurement: allow,
    measurements: allow,
    measurable: allow,
    measurables: allow,
    bot: allow,
    bots: allow,
    agent: allow,
    agents: allow,
    series: allow,
    seriesCollection: allow,
    channel: allow,
    channels: allow,
    stats: allow,
  },
  Mutation: {
    '*': isAuthenticated,
    channelCreate: isAuthenticated,
    userUpdate: isAuthenticated,
    measurementCreate: and(isAuthenticated, or(isChannelPublic, or(isAdmin, isViewer))),
    seriesCreate: and(isAuthenticated, or(isChannelPublic, isAdmin)),

    measurableCreate: and(isAuthenticated, or(isChannelPublic, or(isAdmin, isViewer))),
    measurableArchive: and(isAuthenticated, isOwner),
    measurableUnarchive: and(isAuthenticated, isOwner),
    measurableUpdate: and(isAuthenticated, isOwner),

    ...rulesChannel.Mutation,
    ...rulesChannelMemberships.Mutation,
  }
};

function getPermissions() {
  return shield(
    _.cloneDeep(rules),
    { debug: false }
  );
}

module.exports = {
  rules,
  rulesChannel,
  rulesChannelMemberships,
  getPermissions,
};
