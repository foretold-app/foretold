const { MeasurablesData } = require('./measurables-data');
const { MeasurementsData } = require('./measurements-data');
const { UsersData } = require('./users-data');
const { SeriesData } = require('./series-data');
const { ChannelsData } = require('./channels-data');
const { ChannelMembershipsData } = require('./channel-memberships-data');
const { AgentsData } = require('./agents-data');
const { BotsData } = require('./bots-data');
const { AuthenticationData } = require('./authentication-data');

module.exports = {
  measurables: new MeasurablesData(),
  measurements: new MeasurementsData(),
  users: new UsersData(),
  series: new SeriesData(),
  channels: new ChannelsData(),
  channelMemberships: new ChannelMembershipsData(),
  agents: new AgentsData(),
  bots: new BotsData(),
  authentication: new AuthenticationData(),
};
