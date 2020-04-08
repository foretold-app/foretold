const _ = require('lodash');

const { MeasurementValue } = require('../../lib/measurement-value');
const { MEASURABLE_STATE } = require('../../enums');
const { MEASUREMENT_COMPETITOR_TYPE } = require('../../enums');
const { MEASUREMENT_VALUE } = require('../../enums');
const { MEASURABLE_VALUE_TYPE } = require('../../enums');
const lang = require('../../../config/lang');

/**
 * @param {*} root
 * @param {object} args
 * @param {object} args.input
 * @param {object} args.input.value
 * @param {Schema.Context} _context
 * @param {object} _info
 * @return {Promise<boolean>}
 */
async function measurementValueValidation(root, args, _context, _info) {
  const value = _.get(args, ['input', 'value'], null);
  return new MeasurementValue(value).validate();
}

/**
 * Before split this function onto logic functions think twice.
 * @param {*} root
 * @param {object} args
 * @param {Schema.Context} context
 * @param {object} _info
 * @return {Promise<boolean>}
 */
async function competitiveMeasurementCanBeAddedToOpenMeasurable(
  root, args, context, _info,
) {
  const measurementType = _.get(args, 'input.competitorType', null);
  const isCompetitive
    = MEASUREMENT_COMPETITOR_TYPE.COMPETITIVE === measurementType;

  const measurableState = _.get(context, 'measurable.state', '');
  const isMeasurableAvailable = [
    MEASURABLE_STATE.OPEN,
    MEASURABLE_STATE.JUDGEMENT_PENDING,
  ].includes(measurableState);

  if (!isCompetitive) return true;
  if (!isMeasurableAvailable) throw new Error(lang.measurableIsNotOpen());

  return true;
}

/**
 * @param {*} root
 * @param {object} args
 * @param {object} args.input
 * @param {object} args.input.value
 * @param {Schema.Context} context
 * @param {object} _info
 * @return {Promise<boolean>}
 */
async function measurementValueTypeValidation(root, args, context, _info) {
  const inputValue = _.get(args, ['input', 'value'], null);
  const type = _.get(context, 'measurable.valueType', '');

  if (!type) throw new Error(lang.measurableValueType());

  const value = new MeasurementValue(inputValue);

  if (type === MEASURABLE_VALUE_TYPE.FLOAT) {
    if (value.getType() === MEASUREMENT_VALUE.floatCdf) return true;
    if (value.getType() === MEASUREMENT_VALUE.floatPoint) return true;
  }
  if (type === MEASURABLE_VALUE_TYPE.PERCENTAGE) {
    if (value.getType() === MEASUREMENT_VALUE.binary) return true;
    if (value.getType() === MEASUREMENT_VALUE.percentage) return true;
  }
  if (value.getType() === MEASUREMENT_VALUE.unresolvableResolution) return true;
  if (value.getType() === MEASUREMENT_VALUE.comment) return true;

  throw new Error(lang.measurementValueTypeWrong());
}

module.exports = {
  competitiveMeasurementCanBeAddedToOpenMeasurable,
  measurementValueTypeValidation,
  measurementValueValidation,
};
