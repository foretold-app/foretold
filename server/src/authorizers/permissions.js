const { shield, allow, and, or } = require('graphql-shield');

const { isAuthenticated } = require('./users');
const { isAdmin, isViewer } = require('./channel-memberships');
const { isChannelPublic } = require('./channels');
const { isOwner } = require('./measurables');

function getPermissions() {
  return shield(
    {
      Query: {
        '*': allow,
      },
      Mutation: {
        '*': isAuthenticated,
        measurementCreate: and(isAuthenticated, or(isChannelPublic, or(isAdmin, isViewer))),
        measurableCreate: and(isAuthenticated, or(isChannelPublic, or(isAdmin, isViewer))),
        channelUpdate: and(isAuthenticated, isAdmin),

        channelMembershipCreate: and(isAuthenticated, isAdmin),
        channelMembershipDelete: and(isAuthenticated, isAdmin),
        channelMembershipRoleUpdate: and(isAuthenticated, isAdmin),

        leaveChannel: and(isAuthenticated),
        joinChannel: and(isAuthenticated, isChannelPublic),

        seriesCreate: and(isAuthenticated, or(isChannelPublic, isAdmin)),
        measurableArchive: and(isAuthenticated, isOwner),
        measurableUnarchive: and(isAuthenticated, isOwner),
        measurableUpdate: and(isAuthenticated, isOwner),
      }
    },
    { debug: false }
  );
}

module.exports = {
  getPermissions,
};
