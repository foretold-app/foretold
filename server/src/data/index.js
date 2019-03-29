const { MeasurablesData } = require('./measurables-data');
const { MeasurementData } = require('./measurement-data');
const { UsersData } = require('./users-data');
const { SeriesData } = require('./series-data');
const { ChannelsData } = require('./channels-data');
const { AgentsChannelsData } = require('./agents-channels-data');

module.exports = {
  measurablesData: new MeasurablesData(),
  measurementData: new MeasurementData(),
  usersData: new UsersData(),
  seriesData: new SeriesData(),
  channelsData: new ChannelsData(),
  agentsChannelsData: new AgentsChannelsData(),
};
