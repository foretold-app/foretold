const R = require('ramda');
const models = require("../../models/index");

const average = R.converge(R.divide, [R.sum, R.length])

async function makeAggregation(measurable) {
  const measurements = await measurable.getMeasurements();
  const percentile25 = average(measurements.map(m => m.dataValues.percentile25));
  const percentile50 = average(measurements.map(m => m.dataValues.percentile50));
  const percentile75 = average(measurements.map(m => m.dataValues.percentile75));
  const newMeasurement = await models.Measurement.create({
    percentile25,
    percentile50,
    percentile75,
    isAggregation: true,
    measurableId: measurable.dataValues.id
  })
}

module.exports = { makeAggregation };
