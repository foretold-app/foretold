const { shield, allow, and } = require('graphql-shield');

const users = require('./users');
const agentsChannels = require('./agents-channels');

function getPermissions() {
  return shield({
    Query: {
      '*': allow,
      user: users.isAuthenticated,
    },
    Mutation: {
      '*': users.isAuthenticated,
      channelUpdate: and(users.isAuthenticated, agentsChannels.isAdmin),
      agentsChannelsCreate: and(users.isAuthenticated, agentsChannels.isAdmin),
      agentsChannelsDelete: and(users.isAuthenticated, agentsChannels.isAdmin),
      agentsChannelsUpdate: and(users.isAuthenticated, agentsChannels.isAdmin),
    }
  });
}

module.exports = {
  getPermissions,
};
