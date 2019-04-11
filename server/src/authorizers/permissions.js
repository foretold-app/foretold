const _ = require('lodash');
const { shield, allow, and, or } = require('graphql-shield');

const { isAuthenticated } = require('./users');
const { isAdmin, isViewer } = require('./channel-memberships');
const { isChannelPublic } = require('./channels');
const { isOwner } = require('./measurables');

const rules = {
  Query: {
    '*': allow,
  },
  Mutation: {
    '*': isAuthenticated,
    measurementCreate: and(isAuthenticated, or(isChannelPublic, or(isAdmin, isViewer))),
    seriesCreate: and(isAuthenticated, or(isChannelPublic, isAdmin)),

    channelMembershipCreate: and(isAuthenticated, isAdmin),
    channelMembershipDelete: and(isAuthenticated, isAdmin),
    channelMembershipRoleUpdate: and(isAuthenticated, isAdmin),

    channelUpdate: and(isAuthenticated, isAdmin),
    leaveChannel: and(isAuthenticated),
    joinChannel: and(isAuthenticated, isChannelPublic),

    measurableCreate: and(isAuthenticated, or(isChannelPublic, or(isAdmin, isViewer))),
    measurableArchive: and(isAuthenticated, isOwner),
    measurableUnarchive: and(isAuthenticated, isOwner),
    measurableUpdate: and(isAuthenticated, isOwner),
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
  getPermissions,
};
