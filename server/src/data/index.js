const { MeasurablesData } = require('./measurables-data');
const { MeasurementData } = require('./measurement-data');
const { UsersData } = require('./users-data');
const { SeriesData } = require('./series-data');
const { ChannelsData } = require('./channels-data');
const { ChannelsMembershipsData } = require('./channels-memberships-data');

module.exports = {
  measurablesData: new MeasurablesData(),
  measurementData: new MeasurementData(),
  usersData: new UsersData(),
  seriesData: new SeriesData(),
  channelsData: new ChannelsData(),
  channelsMembershipsData: new ChannelsMembershipsData(),
};
