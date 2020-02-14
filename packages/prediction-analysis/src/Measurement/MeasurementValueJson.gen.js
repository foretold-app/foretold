/* Untyped file generated from MeasurementValueJson.rei by genType. */

const MeasurementValueJsonBS = require('./MeasurementValueJson.bs');

const toMeasurementValue = function (Arg1) {
  const result = MeasurementValueJsonBS.toMeasurementValue(Arg1);
  return result.tag===0
    ? {tag:"Ok", value:result[0]}
    : {tag:"Error", value:result[0]}
};;
exports.toMeasurementValue = toMeasurementValue
