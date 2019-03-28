const { shield, allow, and, or } = require('graphql-shield');

const { isAuthenticated } = require('./users');
const { isAdmin, isViewer } = require('./agents-channels');
const { isChannelAllowed } = require('./channels');

function getPermissions() {
  return shield({
    Query: {
      '*': allow,
      user: isAuthenticated,
    },
    Mutation: {
      '*': isAuthenticated,
      channelUpdate: and(isAuthenticated, or(isChannelAllowed, isAdmin)),
      createSeries: and(isAuthenticated, or(isChannelAllowed, isAdmin)),
      agentsChannelsCreate: and(isAuthenticated, or(isChannelAllowed, isAdmin)),
      agentsChannelsDelete: and(isAuthenticated, or(isChannelAllowed, isAdmin)),
      agentsChannelsUpdate: and(isAuthenticated, or(isChannelAllowed, isAdmin)),
      createMeasurement: and(isAuthenticated, or(isChannelAllowed, or(isAdmin, isViewer))),
    }
  });
}

module.exports = {
  getPermissions,
};
