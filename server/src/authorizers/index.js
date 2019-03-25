const { shield } = require('graphql-shield');

const { isChannelAllowed } = require('./channels');

const permissions = shield({
  Query: {
    // user: not(isAuthenticated),
    // users: not(isAuthenticated),
    measurement: isChannelAllowed,
    measurements: isChannelAllowed,
    measurable: isChannelAllowed,
    measurables: isChannelAllowed,
    // bot: not(isAuthenticated),
    // bots: not(isAuthenticated),
    // agent: not(isAuthenticated),
    // agents: not(isAuthenticated),
    series: isChannelAllowed,
    seriesCollection: isChannelAllowed,
    // channel: not(isAuthenticated),
    // channels: not(isAuthenticated),
    // stats: not(isAuthenticated),
  },
  Mutation: {
    // createMeasurement: isAuthenticated,
    // createMeasurable: isAuthenticated,
    // createSeries: isAuthenticated,
    // archiveMeasurable: isAuthenticated,
    // unArchiveMeasurable: isAuthenticated,
    // editMeasurable: isAuthenticated,
    // editUser: isAuthenticated,
    // channelUpdate: isAuthenticated,
    // channelCreate: isAuthenticated,
    // agentsChannelsCreate: isAuthenticated,
    // agentsChannelsDelete: isAuthenticated,
  },
});

module.exports = {
  permissions,
};
