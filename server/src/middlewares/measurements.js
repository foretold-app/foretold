const _ = require('lodash');

const data = require('../data');

async function measurement(root, args, context, info) {
  const measurementId = _.get(args, 'id')
    || _.get(root, 'measurementId')
    || _.get(context, 'measurementId');
  console.log('\x1b[36m ---> \x1b[0m Middleware (measurement)', { measurementId });
  if (!measurementId) return;
  const measurement = await data.measurementData.getOne(measurementId);
  context.measurement = measurement;
  context.measurableId = measurement.measurableId;
}

module.exports = {
  measurement,
};
