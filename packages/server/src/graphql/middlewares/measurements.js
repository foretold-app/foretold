const _ = require('lodash');

const { MeasurementValue } = require('../../lib/measurement-value');
const { MEASUREMENT_VALUE } = require('../../lib/measurement-value/enums');

const { MeasurementsData } = require('../../data');

const { Params } = require('../../data/classes');
const { Query } = require('../../data/classes');
const { Options } = require('../../data/classes');

const { MEASURABLE_STATE } = require('../../enums');
const { MEASUREMENT_COMPETITOR_TYPE } = require('../../enums');
const { MEASURABLE_VALUE_TYPE } = require('../../enums');
const lang = require('../../../config/lang');
const logger = require('../../lib/log');

const log = logger.module('middlewares/measurement');

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

/**
 * @param {object | null} root
 * @param {object} args
 * @param {Schema.Context} context
 * @param {object} _info
 * @return {Promise<void>}
 */
async function setContextMeasurement(root, args, context, _info) {
  const measurementId = _.get(args, 'measurementId', null);

  log.trace(
    '\x1b[36m ---> \x1b[0m Middleware (setContextMeasurement)',
    { measurementId },
  );

  if (!!measurementId) {
    const params = new Params({ id: measurementId });
    const query = new Query();
    const options = new Options({ raw: true });
    context.measurement = await new MeasurementsData().getOne(
      params, query, options,
    );
  } else {
    context.measurement = null;
  }
}

/**
 * @param {object | null} root
 * @param {object} args
 * @param {Schema.Context} context
 * @param {object} _info
 * @return {Promise<void>}
 */
async function setContextMeasurementByRoot(root, args, context, _info) {
  log.trace('\x1b[36m ---> \x1b[0m Middleware (setContextMeasurementByRoot)');
  context.measurement = root || null;
}

module.exports = {
  competitiveMeasurementCanBeAddedToOpenMeasurable,
  measurementValueTypeValidation,
  measurementValueValidation,

  setContextMeasurement,
  setContextMeasurementByRoot,
};
