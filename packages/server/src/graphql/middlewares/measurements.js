const _ = require('lodash');

const { MeasurementValue } = require('@foretold/measurement-value');
const { FloatPoint, FloatCdf } = require('@foretold/measurement-value');
const { Binary, Percentage } = require('@foretold/measurement-value');
const { UnresolvableResolution } = require('@foretold/measurement-value');
const { Comment } = require('@foretold/measurement-value');

const { MEASURABLE_STATE } = require('../../enums');
const { MEASUREMENT_COMPETITOR_TYPE } = require('../../enums');
const { MEASURABLE_VALUE_TYPE } = require('../../enums');
const lang = require('../../lang');

/**
 * @param {*} root
 * @param {object} args
 * @param {object} args.input
 * @param {object} args.input.value
 * @param {Schema.Context} context
 * @param {object} info
 * @return {Promise<boolean>}
 */
async function measurementValueValidation(root, args, context, info) {
  const value = _.get(args, ['input', 'value'], null);
  return MeasurementValue.factory(value).validate();
}

/**
 * @param {*} root
 * @param {object} args
 * @param {Schema.Context} context
 * @param {object} info
 * @return {Promise<boolean>}
 */
async function measurableStateValidation(root, args, context, info) {
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
 * @param {object} info
 * @return {Promise<boolean>}
 */
async function measurementTypeValidation(root, args, context, info) {
  const inputValue = _.get(args, ['input', 'value'], null);
  const type = _.get(context, 'measurable.valueType', '');
  if (!type) throw new Error(lang.measurableValueType());

  const value = MeasurementValue.factory(inputValue);

  if (type === MEASURABLE_VALUE_TYPE.FLOAT) {
    if (value instanceof FloatCdf) return true;
    if (value instanceof FloatPoint) return true;
  }
  if (type === MEASURABLE_VALUE_TYPE.PERCENTAGE) {
    if (value instanceof Binary) return true;
    if (value instanceof Percentage) return true;
  }
  if (value instanceof UnresolvableResolution) return true;
  if (value instanceof Comment) return true;

  throw new Error(lang.measurementValueTypeWrong());
}

module.exports = {
  measurableStateValidation,
  measurementValueValidation,
  measurementTypeValidation,
};
