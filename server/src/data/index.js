const { MeasurablesData } = require('./measurables-data');
const { MeasurementData } = require('./measurement-data');
const { UsersData } = require('./users-data');
const { SeriesData } = require('./series-data');

module.exports = {
  measurablesData: new MeasurablesData(),
  measurementData: new MeasurementData(),
  usersData: new UsersData(),
  seriesData: new SeriesData(),
};
