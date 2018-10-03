"use strict";

Object.defineProperty(exports, "__esModule", {
  value: true
});
exports.makeAggregation = makeAggregation;

var _index = require("../../models/index");

var models = _interopRequireWildcard(_index);

function _interopRequireWildcard(obj) { if (obj && obj.__esModule) { return obj; } else { var newObj = {}; if (obj != null) { for (var key in obj) { if (Object.prototype.hasOwnProperty.call(obj, key)) newObj[key] = obj[key]; } } newObj.default = obj; return newObj; } }

const R = require('ramda');


const average = R.converge(R.divide, [R.sum, R.length]);

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
  });
}
//# sourceMappingURL=MakeAggregation.js.map