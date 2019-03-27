const { shield, allow } = require('graphql-shield');

const { isAuthenticated } = require('./users');

function getPermissions() {
  return shield({
    Query: {
      user: isAuthenticated,
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
      createMeasurement: isAuthenticated,
      createMeasurable: isAuthenticated,
      createSeries: isAuthenticated,
      archiveMeasurable: isAuthenticated,
      unArchiveMeasurable: isAuthenticated,
      editMeasurable: isAuthenticated,
      editUser: isAuthenticated,
      channelUpdate: isAuthenticated,
      channelCreate: isAuthenticated,
      agentsChannelsCreate: isAuthenticated,
      agentsChannelsDelete: isAuthenticated,
    }
  });
}

module.exports = {
  getPermissions,
};
