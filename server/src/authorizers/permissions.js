const { shield, allow, and } = require('graphql-shield');

const users = require('./users');
const agentsChannels = require('./agents-channels');

function getPermissions() {
  return shield({
    Query: {
      user: users.isAuthenticated,
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
      createMeasurement: users.isAuthenticated,
      createMeasurable: users.isAuthenticated,
      createSeries: users.isAuthenticated,
      archiveMeasurable: users.isAuthenticated,
      unArchiveMeasurable: users.isAuthenticated,
      editMeasurable: users.isAuthenticated,
      editUser: users.isAuthenticated,
      channelUpdate: users.isAuthenticated,
      channelCreate: users.isAuthenticated,
      agentsChannelsCreate: and(users.isAuthenticated, agentsChannels.isAdmin),
      agentsChannelsDelete: and(users.isAuthenticated, agentsChannels.isAdmin),
      agentsChannelsUpdate: and(users.isAuthenticated, agentsChannels.isAdmin),
    }
  });
}

module.exports = {
  getPermissions,
};
