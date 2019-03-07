const { MeasurableData } = require('./measurable-data');
const { MeasurementData } = require('./measurement-data');
const { UsersData } = require('./users-data');

module.exports = {
  measurableData: new MeasurableData(),
  measurementData: new MeasurementData(),
  usersData: new UsersData(),
};
