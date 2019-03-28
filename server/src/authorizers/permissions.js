const { shield, allow, and, or } = require('graphql-shield');

const { isAuthenticated } = require('./users');
const { isAdmin, isViewer } = require('./agents-channels');
const { isChannelPublic } = require('./channels');

function getPermissions() {
  return shield({
    Query: {
      '*': allow,
      user: isAuthenticated,
    },
    Mutation: {
      '*': isAuthenticated,
      channelUpdate: and(isAuthenticated, or(isChannelPublic, isAdmin)),
      createSeries: and(isAuthenticated, or(isChannelPublic, isAdmin)),
      agentsChannelsCreate: and(isAuthenticated, or(isChannelPublic, isAdmin)),
      agentsChannelsDelete: and(isAuthenticated, or(isChannelPublic, isAdmin)),
      agentsChannelsUpdate: and(isAuthenticated, or(isChannelPublic, isAdmin)),
      createMeasurement: and(isAuthenticated, or(isChannelPublic, or(isAdmin, isViewer))),
    }
  });
}

module.exports = {
  getPermissions,
};
